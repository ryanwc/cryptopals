# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
set (CMAKE_TOOLCHAIN_FILE "/mnt/hgfs/cs6210_AOS/vcpkg/scripts/buildsystems/vcpkg.cmake")
set (CMAKE_CXX_STANDARD 11)

project(hex_to_base64)

set(SOURCE_FILES main.cpp)

add_executable(hex_to_base64 ${SOURCE_FILES})
