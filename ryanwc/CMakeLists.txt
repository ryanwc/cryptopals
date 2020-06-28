# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
set (CMAKE_TOOLCHAIN_FILE "/mnt/hgfs/cs6210_AOS/vcpkg/scripts/buildsystems/vcpkg.cmake")
set (CMAKE_CXX_STANDARD 11)

project(cryptopals)
add_subdirectory("./set1/hex_to_base64")
add_subdirectory("./set1/base64_XOR")

# put binaries in /bin
set_target_properties( hex_to_base64 test_hex_to_base64 base64_XOR test_base64_XOR
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin"
)
