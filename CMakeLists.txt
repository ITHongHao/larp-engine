# at least CMake 3.28.3 should be good
cmake_minimum_required(VERSION 3.28.3)

project(LarpEngine)

set(MAIN src/Main/engine.cpp)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
# set(ENV{DISPLAY}:0.0)

find_package(OpenGL REQUIRED)
include(FetchContent)

# OS Checks, nothing yet
if(WIN32)
endif()
if(MSVC)
endif()
if(APPLE)
endif()
if(UNIX)
endif()

# If you need to get warmed up or used to imgui, uncomment the imgui_demo.cpp line and build CMake again.
# There's a couple lines of warning and awareness in the beginning of imgui_demo.cpp, please read there.
set(IMGUI_SRC
      vendor/imgui-1.92.9b/imgui.cpp
      vendor/imgui-1.92.9b/imgui_draw.cpp
      vendor/imgui-1.92.9b/imgui_tables.cpp
      vendor/imgui-1.92.9b/imgui_widgets.cpp
      #vendor/imgui-1.92.9b/imgui_demo.cpp
      vendor/imgui-1.92.9b/backends/imgui_impl_opengl3.cpp
      vendor/imgui-1.92.9b/backends/imgui_impl_sdl3.cpp
    )

#[========================================================================================================================================[
IMGUI NOTE:
  imgui has no cmakelists.txt in its repo,
    and package managers would make it difficult to alterations to imgui design
    and collaboration with certain other libraries and packages
  #define IMGUI_IMPLEMENTATION is disabled, as misc/single_file/imgui_single_file.h is deleted.
  imconfig.h would likely have to be altered to be compatible with certain libraries

STB_IMAGE NOTE: A quote from https://github.com/nothings/stb, on why stb_image is manually added into the repo:
  "The idea behind single-header file libraries is that they're easy to distribute
    and deploy because all the code is contained in a single file.
    [...]
    For example, to use stb_image, you should have exactly one C/C++ file that doesn't include stb_image.h regularly, but instead does
      #define STB_IMAGE_IMPLEMENTATION
      include "stb_image.h""

PUGIXML NOTE: From https://pugixml.org/docs/quickstart.html:
  Just download the latest source distribution as an archive and extract it.
]========================================================================================================================================]
add_library(IMGUI STATIC ${IMGUI_SRC})
add_library(STB_IMAGE STATIC "vendor/stb_image.cpp")
add_library(XML STATIC "vendor/pugixml.cpp")
target_include_directories(IMGUI PUBLIC vendor/imgui-1.92.9b vendor/imgui-1.92.9b/backends)
target_include_directories(STB_IMAGE PUBLIC vendor)
target_include_directories(XML PUBLIC vendor)


FetchContent_Declare(
  SDL3
  GIT_REPOSITORY https://github.com/libsdl-org/SDL
  GIT_TAG release-3.4.18
)

FetchContent_Declare(
  EnTT
  GIT_REPOSITORY https://github.com/skypjack/entt
  GIT_TAG v4.0.0
)

FetchContent_Declare(
  glm
  GIT_REPOSITORY https://github.com/g-truc/glm
  GIT_TAG 1.0.3
)

FetchContent_Declare(
  glad
  GIT_REPOSITORY https://github.com/Dav1dde/glad
  GIT_TAG v2.0.8
  SOURCE_SUBDIR cmake
)

FetchContent_MakeAvailable(SDL3 EnTT glm glad)

#glad2 generation
glad_add_library(glad STATIC API gl:core=3.3)

target_link_libraries(IMGUI PRIVATE SDL3::SDL3 OpenGL::GL)
add_executable(${PROJECT_NAME} ${MAIN})
target_link_libraries(${PROJECT_NAME} PRIVATE SDL3::SDL3 EnTT::EnTT glm::glm glad OpenGL::GL IMGUI STB_IMAGE XML
)

# an executable command to keep things next to each other for the project to be easily found by the OS.
if (WIN32)
    add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD # POST_BUILD: execute this command after .exe is completedly built
        COMMAND ${CMAKE_COMMAND} -E copy_if_different # copies file from one place to another if file doesn't exist at that destination or if source file is updated.
        $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}> # generates list of every .dll file the project requires
        $<TARGET_FILE_DIR:${PROJECT_NAME}> # finds folder path where the executable should land
        COMMAND_EXPAND_LISTS # do consecutive reading, not simulateous for when there's multiple .dll files
    )
endif()