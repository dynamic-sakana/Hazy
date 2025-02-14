cmake_minimum_required(VERSION 3.20.0)
project(Sandbox VERSION 0.1.0 LANGUAGES C CXX)


add_subdirectory("../Hazy" "../Hazy")


file(GLOB_RECURSE sourceFiles RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.cpp" "*.c")
# 递归查找当前目录及子目录下的所有头文件
file(GLOB_RECURSE headerFiles RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} "*.h" "*.hpp")
# 遍历找到的头文件，提取其目录并添加到变量中
foreach(headerFile ${headerFiles})
    get_filename_component(HEADER_DIR ${headerFile} DIRECTORY)
    list(APPEND includeDir ${HEADER_DIR})
endforeach()


add_executable(${PROJECT_NAME} ${sourceFiles})
target_include_directories(${PROJECT_NAME} PRIVATE ${includeDir})
target_link_libraries(${PROJECT_NAME} PRIVATE Hazy)