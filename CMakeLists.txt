cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(ControllerKit)


set(CMAKE_SUPPRESS_REGENERATION true)

set(src_include
	include/ControllerKit.hxx
	include/DualSense.hxx
	include/Math.hxx
)

set(src
	src/ControllerKit.cxx
	src/DualSense.cxx
)

add_executable(DualSense4Windows ${src} ${src_include})
#target_link_directories(Win64 PUBLIC lib/)
target_link_libraries(DualSense4Windows ${CMAKE_SOURCE_DIR}/lib/hidapi.lib)
target_link_libraries(DualSense4Windows setupapi)
target_link_libraries(DualSense4Windows ${CMAKE_SOURCE_DIR}/lib/ViGEmClient.lib)
target_include_directories(DualSense4Windows PUBLIC external/)
target_include_directories(DualSense4Windows PRIVATE include/)

