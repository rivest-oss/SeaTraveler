cmake_minimum_required(VERSION 3.15)

project(seatraveler)
set(CMAKE_CXX_STANDARD 17)

include(FetchContent)

function(include_dependency depName gitURL gitTag)
	FetchContent_Declare(${depName}
		GIT_REPOSITORY	${gitURL}
		GIT_TAG			${gitTag}
		GIT_SHALLOW		TRUE
		GIT_PROGRESS	TRUE
	)
	
	FetchContent_MakeAvailable(${depName})
endfunction()

find_package(SDL3 QUIET)
if(NOT SDL3_FOUND)
	message(STATUS "Downloading SDL3 from GitHub")
	include_dependency(SDL3 https://github.com/libsdl-org/SDL.git release-3.2.x)
else()
	message(STATUS "Using local SDL3")
endif()

find_package(SDL3_net QUIET)
if(NOT SDL3_net_FOUND)
	message(STATUS "Downloading SDL3_net from GitHub")
	include_dependency(SDL3_net https://github.com/libsdl-org/SDL_net.git release-3.2.x)
else()
	message(STATUS "Using local SDL3_net")
endif()

set(SDLTTF_VENDORED ON)
find_package(SDL3_ttf QUIET)
if(NOT SDL3_ttf_FOUND)
	message(STATUS "Downloading SDL3_ttf from GitHub")
	include_dependency(SDL3_ttf https://github.com/libsdl-org/SDL_ttf.git release-3.2.x)
else()
	message(STATUS "Using local SDL3_ttf")
endif()

if(SDL3_net_FOUND AND NOT TARGET SDL3::SDL3_net)
	add_library(SDL3::SDL3_net INTERFACE IMPORTED)
	set_target_properties(
		SDL3::SDL3_net
		PROPERTIES
			INTERFACE_INCLUDE_DIRECTORIES "${SDL3_NET_INCLUDE_DIRS}"
			INTERFACE_LINK_LIBRARIES "${SDL3_NET_LIBRARIES}"
	)
endif()

# Programme
file(GLOB_RECURSE sources CONFIGURE_DEPENDS "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp")
list(REMOVE_ITEM sources "${CMAKE_CURRENT_SOURCE_DIR}/src/excluded.cpp")

add_executable(${PROJECT_NAME} "${sources}")
target_include_directories(${PROJECT_NAME} PUBLIC ${SDL3_INCLUDE_DIRS} ${SDL3_NET_INCLUDE_DIRS} ${SDL3_TTF_INCLUDE_DIRS})
target_compile_definitions(${PROJECT_NAME} PRIVATE QPURIQ_ENABLE_BACKEND_SDL3)
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3 SDL3_net::SDL3_net SDL3_ttf::SDL3_ttf)

if(WIN32)
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND "${CMAKE_COMMAND}" -E copy_if_different "$<TARGET_FILE:SDL3::SDL3>" "$<TARGET_FILE_DIR:${PROJECT_NAME}>"
		VERBATIM
	)
endif()
