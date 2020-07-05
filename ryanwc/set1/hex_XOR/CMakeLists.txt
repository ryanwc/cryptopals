# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
set (CMAKE_TOOLCHAIN_FILE "/mnt/hgfs/cs6210_AOS/vcpkg/scripts/buildsystems/vcpkg.cmake")
set (CMAKE_CXX_STANDARD 11)

project(hex_XOR)

# uses a function from challenge 1
set(NON_MAIN_SOURCE_FILES hex_XOR.cpp hex_XOR.h ../../crypto_lib/uint64_bits.cpp ../../crypto_lib/uint64_bits.h)
set(SOURCE_FILES main.cpp ${NON_MAIN_SOURCE_FILES})


# the actual program
add_executable(hex_XOR ${SOURCE_FILES})


# link against gtest or gtest_main as needed
set(TEST_SOURCE_FILES test_main.cpp test_hex_XOR.cpp ${NON_MAIN_SOURCE_FILES})

add_executable(test_hex_XOR ${TEST_SOURCE_FILES})

target_link_libraries(test_hex_XOR gtest gtest_main pthread)
add_test(test_hex_XOR test_hex_XOR)
