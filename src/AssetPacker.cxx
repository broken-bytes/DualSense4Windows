#include <map>

#include <gumbo.h>
#include <string>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <regex>

#include "../include/Base64.hxx"

namespace fs = std::filesystem;

constexpr char* TARGET = "../include/Assets.hxx";

constexpr char* HTML = "../assets/html";
constexpr char* CSS = "../assets/css";
constexpr char* JS = "../assets/js";
constexpr char* IMG = "../assets/img";

auto GENERATED_ASSETS = std::map<std::string, std::string>();

enum class LinkType {
	STYLES
};

enum class FileType {
	CSS,
	SCRIPT,
	IMG
};

std::string ReplacePath(std::string path, FileType type) {
	std::string str;
	std::string img = "../img";
	std::string css = "../css";
	std::string js = "../js";
	switch (type) {
	case FileType::CSS:
		return std::regex_replace(path, std::regex(css), CSS);
	case FileType::SCRIPT:
		return std::regex_replace(path, std::regex(js), JS);
	case FileType::IMG:
		return std::regex_replace(path, std::regex(img), IMG);
	}
}

std::string ReplaceCSS(GumboNode* node) {
	GumboAttribute* href = gumbo_get_attribute(&node->v.element.attributes, "href");
	std::ifstream ifs(ReplacePath(href->value, FileType::CSS));
	std::string content((std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>()));
	return "<style>" + content + "</style>";
}

std::string ReplaceIMG(GumboNode* node) {
	GumboAttribute* src = gumbo_get_attribute(&node->v.element.attributes, "src");
	auto file = ReplacePath(src->value, FileType::IMG);
	std::ifstream ifs(file, std::ios_base::binary);
	std::string content((std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>()));
	
	if(file.find(".png") != std::string::npos ||
		file.find(".PNG") != std::string::npos) {
		return "<img src=\"data:image/png;base64, " + base64_encode(
			content.c_str(),
			content.length()
		) + "\"/>";
	}
	if (file.find(".jpg") != std::string::npos ||
		file.find(".JPG") != std::string::npos || 
		file.find(".jpeg") != std::string::npos ||
		file.find(".JPEG") != std::string::npos
	) {
		return "<img src=\"data:image/jpeg;base64, " + base64_encode(
			content.c_str(),
			content.length()
		) + "\"/>";
	}
	
	return "<img src=\"data:image/png;base64, " + base64_encode(
		content.c_str(),
		content.length()
	) + "\"/>";
}

std::string ReplaceJS(GumboNode* node) {
	GumboAttribute* src = gumbo_get_attribute(&node->v.element.attributes, "src");
	std::ifstream ifs(ReplacePath(src->value, FileType::SCRIPT));
	std::string content((std::istreambuf_iterator<char>(ifs)),
		(std::istreambuf_iterator<char>()));
	return "<script>" + content + "</script>";
}

std::string ReplaceNodeContent(GumboNode* node) {
	if (node == nullptr) {
		return "";
	}
	if (node->type != GUMBO_NODE_ELEMENT)
	{
		return "";
	}
	auto attr = node->v.element.attributes;

	std::string str;
	switch (node->v.element.tag) {
	case GUMBO_TAG_IMG:
		return ReplaceIMG(node);
	case GUMBO_TAG_LINK:
		return ReplaceCSS(node);
	case GUMBO_TAG_SCRIPT:
		return ReplaceJS(node);
	default:
		str += "<";
		str += gumbo_normalized_tagname(node->v.element.tag);
		str += " ";
		for(int x = 0; x < attr.length; x++) {
			auto a = static_cast<GumboAttribute*>(attr.data[x]);
			str += a->name;
			str += "=";
			str += "\"";
			str += a->value;
			str += "\"";
		}
		str += ">";
		break;
	}

	GumboVector* children = &node->v.element.children;

	for (unsigned int x = 0; x < children->length; ++x)
	{
		auto child = static_cast<GumboNode*>(children->data[x]);
		str += ReplaceNodeContent(child);
		if(x == children->length - 1) {
			str += "</";
			str += gumbo_normalized_tagname(child->parent->v.element.tag);
			str += ">";
		}
	}
	return str;
}

int main() {
	std::cout << "Starting generation of assets" << std::endl;
	std::ofstream outfile(TARGET, std::ios::out | std::ios::binary);

	outfile << "#pragma once" << std::endl;
	outfile << "#include <string>" << std::endl;
	outfile << "#include <map>" << std::endl;
	outfile << "namespace BrokenBytes::DualSense4Windows::Assets {" << std::endl;
	
	bool isFirstFile = true;

	for (const auto& entry : fs::directory_iterator(HTML)) {
		std::ifstream ifs(entry.path());
		std::string content((std::istreambuf_iterator<char>(ifs)),
			(std::istreambuf_iterator<char>()));
		outfile << "std::string ";
		auto file = entry.path().stem().string();
		outfile << file;
		GumboOutput* out = gumbo_parse(content.c_str());
		GumboNode* node = out->root;
		outfile << " = ";
		outfile << "R\"(";
		auto str = ReplaceNodeContent(node);
		outfile << str;
		outfile << ")\";";
	}

	outfile << "enum class AssetFile {" << std::endl;
	for (const auto& entry : fs::directory_iterator(HTML)) {
		std::ifstream ifs(entry.path());
		std::string content((std::istreambuf_iterator<char>(ifs)),
			(std::istreambuf_iterator<char>()));
		if(!isFirstFile) {
			outfile << "," << std::endl;
		}
		auto file = entry.path().stem().string();
		outfile << file;
		
		std::cout << "Generated " << entry.path() << std::endl;
		isFirstFile = false;
	}

	outfile << "};" << std::endl;;
	
	outfile << "std::string AssetData(AssetFile file) {" << std::endl;
	for (const auto& entry : fs::directory_iterator(HTML)) {
		std::ifstream ifs(entry.path());
		auto file = entry.path().stem().string();
		outfile << "if(file == AssetFile::" << file << ") {" << std::endl;
		outfile << "return " << file << ";" << std::endl;
		outfile << "}" << std::endl;
	}
	outfile << "return \"\";" << std::endl;
	outfile << "}" << std::endl;
	outfile << "}";

	outfile.close();
}