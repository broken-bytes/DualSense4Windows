cmake_minimum_required(VERSION 3.14)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(APP DualSense4Windows)
project(${APP})


set(CMAKE_SUPPRESS_REGENERATION true)

set(SRC_INC
	include/ControllerKit.hxx
	include/DualSense.hxx
	include/Math.hxx
	include/Utils.hxx
	include/App.hxx
	include/Interface.hxx
	include/Types.hxx
	include/Discord.hxx
)

set(SRC
	src/ControllerKit.cxx
	src/DualSense.cxx
	src/Utils.cxx
	src/App.cxx
	src/Interface.cxx
	src/Discord.cxx
)

set(ASSETS "$<TARGET_FILE_DIR:${APP}>/assets") 

add_executable(${APP} WIN32 ${SRC} ${SRC_INC})
target_link_directories(${APP} PUBLIC lib/)
target_link_libraries(${APP} Xinput.lib)
target_link_libraries(${APP} AppCore.lib)
target_link_libraries(${APP} Ultralight.lib)
target_link_libraries(${APP} UltralightCore.lib)
target_link_libraries(${APP} WebCore.lib)
target_link_libraries(${APP} hidapi.lib)
target_link_libraries(${APP} setupapi)
target_link_libraries(${APP} ViGEmClient.lib)
target_link_libraries(${APP} discord_game_sdk.dll.lib)
target_include_directories(${APP} PUBLIC external/)
target_include_directories(${APP} PRIVATE include/)

add_custom_command(TARGET ${APP} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/assets/" "${ASSETS}")

	add_custom_command(TARGET ${APP} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory "${CMAKE_CURRENT_SOURCE_DIR}/bin" "$<TARGET_FILE_DIR:${APP}>")

