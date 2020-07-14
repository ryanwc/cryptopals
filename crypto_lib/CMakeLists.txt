# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
set (CMAKE_TOOLCHAIN_FILE "/mnt/hgfs/cs6210_AOS/vcpkg/scripts/buildsystems/vcpkg.cmake")
set (CMAKE_CXX_STANDARD 17)

project(crypto_lib)

set(SOURCE_FILES uint64_bits.h uint64_bits.cpp)

# link against gtest or gtest_main as needed
set(TEST_SOURCE_FILES test_main.cpp test_uint64_bits.cpp ${SOURCE_FILES})

add_executable(test_crypto_lib ${TEST_SOURCE_FILES})

target_link_libraries(test_crypto_lib gtest gtest_main pthread)
add_test(test_crypto_lib test_crypto_lib)
