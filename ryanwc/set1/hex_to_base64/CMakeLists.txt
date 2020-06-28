# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
set (CMAKE_TOOLCHAIN_FILE "/mnt/hgfs/cs6210_AOS/vcpkg/scripts/buildsystems/vcpkg.cmake")
set (CMAKE_CXX_STANDARD 11)

project(hex_to_base64)

set(SOURCE_FILES main.cpp hex_to_base64.cpp hex_to_base64.h)

# the actual program
add_executable(hex_to_base64 ${SOURCE_FILES})

# link against gtest or gtest_main as needed
set(TEST_SOURCE_FILES test_main.cpp test_hex_to_base64.cpp hex_to_base64.cpp hex_to_base64.h)

add_executable(test_hex_to_base64 ${TEST_SOURCE_FILES})

target_link_libraries(test_hex_to_base64 gtest gtest_main pthread)
add_test(test_hex_to_base64 test_hex_to_base64)
