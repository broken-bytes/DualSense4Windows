cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(ControllerKit)


set(CMAKE_SUPPRESS_REGENERATION true)

set(src_include
	include/ControllerKit.hxx
	include/DualSense.hxx
	include/Math.hxx
	include/Utils.hxx
)

set(src
	src/ControllerKit.cxx
	src/DualSense.cxx
	src/Utils.cxx
)

add_executable(DualSense4Windows WIN32 ${src} ${src_include})
target_link_directories(DualSense4Windows PUBLIC lib/)
target_link_libraries(DualSense4Windows Xinput.lib)
target_link_libraries(DualSense4Windows AppCore.lib)
target_link_libraries(DualSense4Windows ${CMAKE_SOURCE_DIR}/lib/hidapi.lib)
target_link_libraries(DualSense4Windows setupapi)
target_link_libraries(DualSense4Windows ${CMAKE_SOURCE_DIR}/lib/ViGEmClient.lib)
target_include_directories(DualSense4Windows PUBLIC external/)
target_include_directories(DualSense4Windows PRIVATE include/)

