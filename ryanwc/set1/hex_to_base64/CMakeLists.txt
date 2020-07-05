# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
set (CMAKE_TOOLCHAIN_FILE "/mnt/hgfs/cs6210_AOS/vcpkg/scripts/buildsystems/vcpkg.cmake")
set (CMAKE_CXX_STANDARD 11)

project(hex_to_base64)

set(NON_MAIN_SOURCE_FILES hex_to_base64.h hex_to_base64.cpp ../../crypto_lib/uint64_bits.h ../../crypto_lib/uint64_bits.cpp)
set(SOURCE_FILES main.cpp ${NON_MAIN_SOURCE_FILES})

# the actual program
add_executable(hex_to_base64 ${SOURCE_FILES})

# link against gtest or gtest_main as needed
set(TEST_SOURCE_FILES test_main.cpp test_hex_to_base64.cpp ${NON_MAIN_SOURCE_FILES})

add_executable(test_hex_to_base64 ${TEST_SOURCE_FILES})

target_link_libraries(test_hex_to_base64 gtest gtest_main pthread)
add_test(test_hex_to_base64 test_hex_to_base64)
