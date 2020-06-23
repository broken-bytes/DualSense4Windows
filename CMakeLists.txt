cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

project(ControllerKit)


set(CMAKE_SUPPRESS_REGENERATION true)

set(src_include
    include/Controller.hxx
	include/XOne.hxx
	include/DualShock4.hxx
)

set(src
	src/Controller.cxx
	src/XOne.cxx
	src/DualShock4.cxx
	src/Interface.hxx
)

set(src_win
	src/win64/Interface.cxx
)

include_directories(${src_include})
add_library(Win64 STATIC ${src} ${src_win})

