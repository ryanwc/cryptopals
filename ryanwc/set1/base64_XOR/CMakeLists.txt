# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
set (CMAKE_TOOLCHAIN_FILE "/mnt/hgfs/cs6210_AOS/vcpkg/scripts/buildsystems/vcpkg.cmake")
set (CMAKE_CXX_STANDARD 11)

project(base64_XOR)

set(SOURCE_FILES main.cpp base64_XOR.cpp base64_XOR.h)

# the actual program
add_executable(base64_XOR ${SOURCE_FILES})

# link against gtest or gtest_main as needed
set(TEST_SOURCE_FILES test_main.cpp test_base64_XOR.cpp base64_XOR.cpp base64_XOR.h)

add_executable(test_base64_XOR ${TEST_SOURCE_FILES})

target_link_libraries(test_base64_XOR gtest gtest_main pthread)
add_test(test_base64_XOR test_base64_XOR)


# put binaries in /bin
set_target_properties( base64_XOR test_base64_XOR
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin"
)
