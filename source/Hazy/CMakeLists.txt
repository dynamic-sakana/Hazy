cmake_minimum_required(VERSION 3.20.0)
project(Hazy VERSION 0.1.0 LANGUAGES C CXX)

# C++标准必须在20以上
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(glfw3 REQUIRED)
find_package(glad REQUIRED)
find_package(imgui REQUIRED)
find_package(spdlog REQUIRED)
find_package(glm REQUIRED)
find_package(Stb REQUIRED)
find_package(assimp REQUIRED)

# 源文件和头文件目录
file(GLOB_RECURSE sourceFiles RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.cpp" "*.c")
file(GLOB_RECURSE headerFiles RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.h" "*.hpp")
foreach(headerFile ${headerFiles})
    get_filename_component(HEADER_DIR ${headerFile} DIRECTORY)
    list(APPEND includeDir ${HEADER_DIR})
endforeach()
list(REMOVE_DUPLICATES includeDir)

# 链接库列表
list(APPEND linkLibraries
	spdlog::spdlog
	glfw
	glad::glad
	imgui::imgui
	assimp::assimp
	$<$<BOOL:${WIN32}>:opengl32>  # Windows 需要链接 opengl32
)

# 不同平台的选项
if (WIN32)
	add_compile_definitions(WINDOWS_PLATFORM)
endif()

# 编译器选项
if (MSVC)
    add_compile_options(/W4 /utf-8)
	add_compile_definitions(_CRT_SECURE_NO_WARNINGS)
else()
	add_compile_options(-Wall -Wextra -Wpedantic -Werror "-fexec-charset=utf-8")
endif()

# 编译目标
add_library(Hazy STATIC ${sourceFiles})
target_sources(Hazy 
PRIVATE 
	${sourceFiles}
PUBLIC 
	${headerFiles}
)
target_include_directories(Hazy PUBLIC ${includeDir})
target_include_directories(Hazy PRIVATE ${Stb_INCLUDE_DIR})
target_link_directories(Hazy PUBLIC ${linkDir})
target_link_libraries(Hazy PUBLIC ${linkLibraries})
# 预编译头文件
target_precompile_headers(Hazy 
PUBLIC 
	${CMAKE_CURRENT_LIST_DIR}/include/hazy_pch.h
)