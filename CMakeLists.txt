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

#add_library(Win64 STATIC ${src} ${src_win})

add_executable(Win64 ${src} ${src_include})
#target_link_directories(Win64 PUBLIC lib/)
target_link_libraries(Win64 ${CMAKE_SOURCE_DIR}/lib/hidapi.lib)
target_link_libraries(Win64 setupapi)
target_link_libraries(Win64 ${CMAKE_SOURCE_DIR}/lib/ViGEmClient.lib)
target_include_directories(Win64 PUBLIC external/)
target_include_directories(Win64 PRIVATE include/)

