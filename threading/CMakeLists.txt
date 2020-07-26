# CMakeLists.txt
cmake_minimum_required(VERSION 3.10)
set (CMAKE_TOOLCHAIN_FILE "/mnt/hgfs/cs6210_AOS/vcpkg/scripts/buildsystems/vcpkg.cmake")
set (CMAKE_CXX_STANDARD 17)

project(results_pool)

set(SOURCE_FILES results_pool.h)

# link against gtest or gtest_main as needed
set(TEST_SOURCE_FILES test_main.cpp test_results_pool.cpp ${SOURCE_FILES})

add_executable(test_results_pool ${TEST_SOURCE_FILES})

target_link_libraries(test_results_pool gtest gtest_main pthread)
add_test(test_results_pool test_results_pool)
