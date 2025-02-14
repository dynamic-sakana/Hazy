cmake_minimum_required(VERSION 3.20.0)
project(test VERSION 0.1.0 LANGUAGES C CXX)

find_package(GTest REQUIRED)

# 查找源文件和头文件
file(GLOB_RECURSE headerFiles RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.h" "*.hpp")
# 提取头文件目录
foreach(headerFile ${headerFiles})
    get_filename_component(HEADER_DIR ${headerFile} DIRECTORY)
    list(APPEND includeDir ${HEADER_DIR})
endforeach()

# 添加GoogleTest的包含目录
list(APPEND includeDir
    "googletest/googletest/include"
    "googletest/googlemock/include"
)
list(APPEND linkLibrarys
    Hazy
    GTest::gtest GTest::gtest_main GTest::gmock GTest::gmock_main
)


# 创建测试可执行文件
add_executable(ThreadPoolTest tests/ThreadPoolTest.cpp)
target_include_directories(ThreadPoolTest PRIVATE ${includeDir})
target_link_libraries(ThreadPoolTest PRIVATE ${linkLibrarys})
add_test(
    NAME ThreadPoolTest
    COMMAND ThreadPoolTest
)

add_executable(EventQueueTest tests/EventQueueTest.cpp)
target_include_directories(EventQueueTest PRIVATE ${includeDir})
target_link_libraries(EventQueueTest PRIVATE ${linkLibrarys})
add_test(
    NAME EventQueueTest
    COMMAND EventQueueTest
)

add_executable(BufferLayoutTest tests/BufferLayoutTest.cpp)
target_include_directories(BufferLayoutTest PRIVATE ${includeDir})
target_link_libraries(BufferLayoutTest PRIVATE ${linkLibrarys})
add_test(
    NAME BufferLayoutTest
    COMMAND BufferLayoutTest
)