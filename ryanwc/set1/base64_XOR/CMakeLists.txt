# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
set (CMAKE_TOOLCHAIN_FILE "/mnt/hgfs/cs6210_AOS/vcpkg/scripts/buildsystems/vcpkg.cmake")
set (CMAKE_CXX_STANDARD 11)

project(base64_XOR)

set(SOURCE_FILES main.cpp base64_XOR.cpp base64_XOR.h)

# the actual program
add_executable(base64_XOR ${SOURCE_FILES})

##############################
# GTEST
##############################
# googletest setup taken from https://github.com/google/googletest/blob/master/googletest/README.md
# Download and unpack googletest at configure time
configure_file(CMakeLists.txt.in googletest-download/CMakeLists.txt)
execute_process(COMMAND ${CMAKE_COMMAND} -G "${CMAKE_GENERATOR}" .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "CMake step for googletest failed: ${result}")
endif()
execute_process(COMMAND ${CMAKE_COMMAND} --build .
    RESULT_VARIABLE result
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/googletest-download )
if(result)
    message(FATAL_ERROR "Build step for googletest failed: ${result}")
endif()

# Prevent overriding the parent project's compiler/linker settings on Windows
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)

# Add googletest directly to our build. This defines the gtest and gtest_main targets
add_subdirectory(${CMAKE_CURRENT_BINARY_DIR}/googletest-src
                 ${CMAKE_CURRENT_BINARY_DIR}/googletest-build
                 EXCLUDE_FROM_ALL)

# The gtest/gtest_main targets carry header search path dependencies automatically
# when using CMake 2.8.11 or later. Otherwise we have to add them here ourselves.
# I guess I don't need this but putting it in for completeness.
if (CMAKE_VERSION VERSION_LESS 2.8.11)
    include_directories("${gtest_SOURCE_DIR}/include")
endif()

# the test program
# Now simply link against gtest or gtest_main as needed
set(TEST_SOURCE_FILES test_main.cpp test_base64_XOR.cpp base64_XOR.cpp base64_XOR.h)

add_executable(test_base64_XOR ${TEST_SOURCE_FILES})

target_link_libraries(test_base64_XOR gtest gtest_main)
add_test(test_base64_XOR test_base64_XOR)


# put binaries in /bin
set_target_properties( base64_XOR test_base64_XOR
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/bin"
)
