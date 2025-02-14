cmake_minimum_required(VERSION 3.20.0)
project(Hazy VERSION 0.1.0 LANGUAGES C CXX)


# C++标准必须在20以上
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# 设置生成目录
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/output/bin/debug)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/output/lib/debug)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/output/bin/debug)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/output/bin/release)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/output/lib/release)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/output/bin/release)


# 不同平台的选项
if (WIN32)
	add_compile_definitions(WINDOWS_PLATFORM)
endif()


# 编译器选项
if (MSVC)
    add_compile_options(/W4 /utf-8)
else()
	add_compile_options(-Wall -Wextra -Wpedantic -Werror "-fexec-charset=utf-8" -O0)
endif()


set(vendorPath "${CMAKE_SOURCE_DIR}/vendor")


add_subdirectory(source/Sandbox)

enable_testing()
add_subdirectory(source/tests)