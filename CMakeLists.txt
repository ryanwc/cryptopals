# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
set (CMAKE_TOOLCHAIN_FILE "/mnt/hgfs/cs6210_AOS/vcpkg/scripts/buildsystems/vcpkg.cmake")
set (CMAKE_CXX_STANDARD 17)

project(cryptopals)
add_subdirectory("./set1/hex_to_base64")
add_subdirectory("./set1/hex_XOR")
add_subdirectory("./set1/single_byte_XOR_cipher")

add_subdirectory("./crypto_lib")

# put binaries in /bin
set_target_properties( 
    hex_to_base64 test_hex_to_base64 
    hex_XOR test_hex_XOR 
    decode_single_byte_XOR_cipher test_decode_single_byte_XOR_cipher
    test_crypto_lib
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin"
)
