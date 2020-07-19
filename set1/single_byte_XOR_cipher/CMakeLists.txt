# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
set (CMAKE_TOOLCHAIN_FILE "/mnt/hgfs/cs6210_AOS/vcpkg/scripts/buildsystems/vcpkg.cmake")
set (CMAKE_CXX_STANDARD 17)

project(single_byte_XOR_cipher)

set(NON_MAIN_SOURCE_FILES 
    decode_single_byte_XOR_cipher.cpp decode_single_byte_XOR_cipher.h 
    ../../crypto_lib/uint64_bits.cpp ../../crypto_lib/uint64_bits.h
    ../../crypto_lib/cryptolib_constants.h
)
set(SOURCE_FILES main.cpp ${NON_MAIN_SOURCE_FILES})


# the actual program
add_executable(decode_single_byte_XOR_cipher ${SOURCE_FILES})


# link against gtest or gtest_main as needed
set(TEST_SOURCE_FILES test_main.cpp test_decode_single_byte_XOR_cipher.cpp ${NON_MAIN_SOURCE_FILES})

add_executable(test_decode_single_byte_XOR_cipher ${TEST_SOURCE_FILES})

target_link_libraries(test_decode_single_byte_XOR_cipher gtest gtest_main pthread)
add_test(test_decode_single_byte_XOR_cipher test_decode_single_byte_XOR_cipher)
