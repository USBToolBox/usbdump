# CMakeList.txt : CMake project for usbdump, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.16)
cmake_policy(SET CMP0091 NEW)
# cmake_policy(SET CMP0149 NEW)
project ("usbdump" LANGUAGES C CXX VERSION 0.1)
include(FetchContent)

# find static libraries (so MT can work ??)
# set(CMAKE_FIND_LIBRARY_SUFFIXES ".lib")
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_POSITION_INDEPENDENT_CODE ON)
# set(BUILD_SHARED_LIBS OFF)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin")

FetchContent_Declare(json URL https://github.com/nlohmann/json/releases/download/v3.11.2/json.tar.xz)
FetchContent_MakeAvailable(json)

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    add_compile_options(/ZH:SHA_256 /W4 /permissive- /Zc:__cplusplus)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    add_compile_options(-Wall -Werror -fno-permissive)
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    add_compile_options(-Wall -Werror -fno-permissive)
endif()

if ("${CMAKE_BUILD_TYPE}" STREQUAL "Release")
    if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
        add_compile_options(/O2 /Ot /Oi /Ob3 /GL)
        add_link_options(/LTCG)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
        add_compile_options(-O3 -fsanitize=undefined,address)
        add_link_options(-fsanitize=undefined,address)
    elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
        add_compile_options(-O3 -fsanitize=undefined,address -fsanitize-blacklist=C:/Users/dhina/Documents/GitHub/usbdump/fsanitize-blacklist.txt)
        add_link_options(-fsanitize=undefined,address -fsanitize-blacklist=C:/Users/dhina/Documents/GitHub/usbdump/fsanitize-blacklist.txt)
    endif()
endif()

add_compile_definitions(UNICODE)

# Add source to this project's executable.
add_executable (usbdump "usbdump.cpp" "enum.cpp" "devnode.cpp" "usbdump.h" "usbdesc.h" "uvcdesc.h" "uvcview.h")
set_property(TARGET usbdump PROPERTY
                MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

target_link_libraries(usbdump
  PRIVATE
    setupapi
    nlohmann_json::nlohmann_json
)
