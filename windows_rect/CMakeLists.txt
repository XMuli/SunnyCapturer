cmake_minimum_required(VERSION 3.14)

set(project_name WindowsRect)
set(project_version_major 1)
set(project_version_minor 0)
set(project_version_path 0)
set(project_version ${project_version_major}.${project_version_minor}.${project_version_path})
project(${project_name} VERSION ${project_version} LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(BUILD_SHARED_LIBS ON)                     # ON-SHARED 生成 dll; OFF-STATIC 生成lib

set(src_files
    include/windowsrect_global.h
    include/windowsrect.h
    sources/windowsrect.cpp
)

if(APPLE)
elseif(WIN32)
    list(APPEND src_files
        include/ntwindowsrect.h
        sources/ntwindowsrect.cpp)
else()
    list(APPEND src_files
        include/x11windowsrect.h
        sources/x11windowsrect.cpp)
endif()

set(src_root_path ${CMAKE_CURRENT_SOURCE_DIR})                                              # default root path curr path （CMakeList.txt）
file(GLOB_RECURSE source_lists LIST_DIRECTORIES false
    "${src_root_path}/*.cpp"
    "${src_root_path}/*.h")
source_group(TREE ${src_root_path} FILES ${source_lists})

add_library(${project_name} ${src_files})

target_include_directories(${project_name} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)     # .cpp 文件可以直接使用 include "文件名.h"，去掉相对路径
target_compile_definitions(${project_name} PRIVATE WINDOWSRECT_LIBRARY UNICODE _UNICODE)

if(MSVC)
    set_target_properties(${project_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${trage_runtime_output_dir}")
    set_target_properties(${project_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${trage_runtime_output_dir}")
    set_target_properties(${project_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${trage_runtime_output_dir}")
    set_target_properties(${project_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${trage_runtime_output_dir}")

    set_target_properties(${project_name} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_DEBUG "${trage_library_output_dir}")
    set_target_properties(${project_name} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELEASE "${trage_library_output_dir}")
    set_target_properties(${project_name} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_MINSIZEREL "${trage_library_output_dir}")
    set_target_properties(${project_name} PROPERTIES LIBRARY_OUTPUT_DIRECTORY_RELWITHDEBINFO "${trage_library_output_dir}")

    set_target_properties(${project_name} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_DEBUG "${trage_archive_output_dir}")
    set_target_properties(${project_name} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELEASE "${trage_archive_output_dir}")
    set_target_properties(${project_name} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_MINSIZEREL "${trage_archive_output_dir}")
    set_target_properties(${project_name} PROPERTIES ARCHIVE_OUTPUT_DIRECTORY_RELWITHDEBINFO "${trage_archive_output_dir}")

    # 开启 MSVC 终端上的中文不乱码
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8")
    add_compile_options(/source-charset:utf-8 /execution-charset:utf-8)
    # 开启 UNICODE 等常规
    target_compile_definitions(${project_name} PRIVATE
        WIN32_LEAN_AND_MEAN                                                                  # Header files containing only the common APIs
        _CRT_SECURE_NO_WARNINGS                                                              # Unsafe functions such as strcpy, scanf, etc. are allowed
        )
endif()
