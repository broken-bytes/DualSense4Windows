cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(APP DualSense4Windows)
set(QT E:/libs/Qt/6.0.2/6.0.2/msvc2019_64)
set(ASSET_PACKER AssetPacker)
project(${APP})


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
	include/DualSenseEntryWidget.hxx
)

set(SRC
	src/DualSense4Windows.cxx
	src/DualSense.cxx
	src/Utils.cxx
	src/Interface.cxx
	src/MainWindow.cxx
	src/ColorPicker.cxx
	src/DualSenseList.cxx
	src/DualSenseEntryWidget.cxx
	DualSense4Windows.exe.manifest
	DeclareDPIAware.manifest
)

set(ASSETS "$<TARGET_FILE_DIR:${APP}>/assets")


add_executable(${APP} WIN32 ${SRC} ${SRC_INC})
target_link_directories(${APP} PUBLIC lib/)
target_link_directories(${APP} PUBLIC ${QT}/lib)
target_link_libraries(${APP} comctl32.lib)
target_link_libraries(${APP} hidapi.lib)
target_link_libraries(${APP} setupapi.lib)
target_link_libraries(${APP} ViGEmClient.lib)
target_link_libraries(${APP} Qt6Core.lib)
target_link_libraries(${APP} Qt6GUI.lib)
target_link_libraries(${APP} Qt6Widgets.lib)
target_include_directories(${APP} PUBLIC external/)
target_include_directories(${APP} PUBLIC ${QT}/include)
target_include_directories(${APP} PRIVATE include/)


add_custom_command(TARGET ${APP} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/bin" "$<TARGET_FILE_DIR:${APP}>")
