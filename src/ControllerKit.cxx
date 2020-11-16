#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <iostream>
#include <vector>

#include <hidapi/hidapi.h>
#include <ViGEm/Client.h>

#include "../include/ControllerKit.hxx"
#include "../include/DualSense.hxx"


using namespace BrokenBytes::ControllerKit;

namespace BrokenBytes::ControllerKit {
	 std::vector<DualSense*> GetDualSenses() {
		auto devices = std::vector<DualSense*>();
		
		auto* ds = hid_enumerate(SONY, DS);
		while (ds != nullptr) {
			if(ds->interface_number != DS_INTERFACE) {
				ds = ds->next;
				continue;
			}
			devices.emplace_back(new DualSense(ds->path));
			ds = ds->next;
		}
		hid_free_enumeration(ds);
	 	
		return devices;
	}
}


int main() {
	wprintf(L"Starting DualSense DS4 Emulator\n");
	wprintf(L"	->Querying for DualSense devices\n");
	auto ds = GetDualSenses();
	wprintf(L"	->Finished querying for DualSense devices\n");
	wprintf(L"	->Found %zu devices\n", ds.size());
	const auto client = vigem_alloc();
	if (client == nullptr)
	{
		std::cerr << "Uh, not enough memory to do that?!" << std::endl;
		return -1;
	}
	const auto retval = vigem_connect(client);

	if (!VIGEM_SUCCESS(retval))
	{
		std::cerr << "ViGEm Bus connection failed with error code: 0x" << std::hex << retval << std::endl;
		return -1;
	}
	auto target = vigem_target_ds4_alloc();
	
	auto dsPads = std::vector<PVIGEM_TARGET>();
	for(int x = 0; x < ds.size(); x++) {
		dsPads.emplace_back(vigem_target_ds4_alloc());
		const auto pir = vigem_target_add(client, dsPads[x]);
		if (!VIGEM_SUCCESS(pir))
		{
			std::cerr << "Target plugin failed with error code: 0x" << std::hex << pir << std::endl;
			return -1;
		}
		ds[x]->SetCallback([client, dsPads, x](DualSense::DS_REPORT report) {
			DS4_REPORT rep;
			DS4_REPORT_INIT(&rep);
			rep.wButtons = report.wButtons;
			rep.bThumbLX = report.bThumbLX;
			rep.bThumbLY = report.bThumbLY;
			rep.bThumbRX = report.bThumbRX;
			rep.bThumbRY = report.bThumbRY;
			rep.bTriggerL = report.bTriggerL;
			rep.bTriggerR = report.bTriggerR;
			rep.bSpecial = report.bSpecial;
			vigem_target_ds4_update(client, dsPads[x], rep);
		});
	}

	wprintf(L"Initialized DS4 Emulator\n");
	// Loop to keep the app running
	while (true) {
	}
}