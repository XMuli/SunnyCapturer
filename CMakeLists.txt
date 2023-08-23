cmake_minimum_required(VERSION 3.5)
project(Sunny)

set(trage_runtime_output_dir "${CMAKE_CURRENT_SOURCE_DIR}/bin")
set(trage_library_output_dir "${CMAKE_CURRENT_SOURCE_DIR}/bin/libs")
set(trage_archive_output_dir "${CMAKE_CURRENT_SOURCE_DIR}/bin/archives")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${trage_runtime_output_dir}")      # exe 和 dll 运行的输出路径
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${trage_library_output_dir}")      # 动态链接库（DLL、SO 等）的输出目录(实际不生效)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${trage_archive_output_dir}")      # 静态库的输出目录

# 获取 CPU 核心数量，开启 IDE 多核编译
include(ProcessorCount)
ProcessorCount(N)
message("-->Number of available processor cores: ${N}")

if(WIN32)
    if(MSVC)
        set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} /MP")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /MP")
    elseif(MINGW)
        set(CMAKE_MAKE_PROGRAM "${CMAKE_MAKE_PROGRAM} -j${N}")
    endif()
elseif(UNIX)
    add_compile_options(-j${N})
endif()

add_subdirectory(src)
add_subdirectory(xwidgets)
#add_subdirectory(common)
