cmake_minimum_required(VERSION 3.14)

if (MSVC_VERSION GREATER_EQUAL "1900")
    include(CheckCXXCompilerFlag)
    CHECK_CXX_COMPILER_FLAG("/std:c++20" _cpp_latest_flag_supported)
    if (_cpp_latest_flag_supported)
        add_compile_options("/std:c++latest")
    endif()
endif()


set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(APP DualSense4Windows)
##set(ASSET_PACKER AssetPacker)
project(${APP})

find_package(Qt6 REQUIRED COMPONENTS Widgets Core Gui)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

set(CMAKE_SUPPRESS_REGENERATION true)
set(SRC_INC
	include/Core.hxx
	include/DualSense4Windows.hxx
	include/DualSense.hxx
	include/Math.hxx
	include/Utils.hxx
	include/Interface.hxx
	include/Types.hxx
	include/Window.hxx
	include/MainWindow.hxx
	include/ColorPicker.hxx
	include/DualSenseInfo.hxx
	include/DualSenseList.hxx
	include/DualSenseListHeader.hxx
	include/DualSenseListEntry.hxx
)

set(SRC
	src/DualSense4Windows.cxx
	src/DualSense.cxx
	src/Utils.cxx
	src/Interface.cxx
	src/MainWindow.cxx
	src/ColorPicker.cxx
	src/DualSenseList.cxx
	src/DualSenseListHeader.cxx
	src/DualSenseListEntry.cxx
	DualSense4Windows.exe.manifest
	DeclareDPIAware.manifest
)

#set(ASSETS "$<TARGET_FILE_DIR:${APP}>/assets")


add_executable(${APP} WIN32 ${SRC} ${SRC_INC})
target_include_directories(${APP} PUBLIC ${QT_DIR}/msvc2019_64/include)
target_link_directories(${APP} PUBLIC lib/)
target_link_libraries(${APP} comctl32.lib)
target_link_libraries(${APP} hidapi.lib)
target_link_libraries(${APP} setupapi.lib)
target_link_libraries(${APP} ViGEmClient.lib)
target_link_libraries(${APP}     
Qt6::Core
Qt6::Widgets
Qt6::Gui)
target_include_directories(${APP} PUBLIC external/)
target_include_directories(${APP} PUBLIC ${QT}/include)
target_include_directories(${APP} PRIVATE include/)
target_compile_options(${APP} PUBLIC /Zc:__cplusplus)
