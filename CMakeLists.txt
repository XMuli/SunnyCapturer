cmake_minimum_required(VERSION 3.5)
project(Sunny)

add_subdirectory(tripartite/QHotkey)  # 三方库置顶，避免被下面统一的路径修改所干扰

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(WIN32)
    set(trage_runtime_output_dir "${CMAKE_SOURCE_DIR}/bin")
    set(trage_library_output_dir "${CMAKE_SOURCE_DIR}/bin")
    set(trage_archive_output_dir "${CMAKE_SOURCE_DIR}/bin")
    message(STATUS "Now is Windows system")
elseif(APPLE)
    message(STATUS "Now is MacOS system")
elseif(UNIX)
    set(trage_runtime_output_dir "${CMAKE_SOURCE_DIR}/bin")
    set(trage_library_output_dir "${CMAKE_SOURCE_DIR}/bin/library")
    set(trage_archive_output_dir "${CMAKE_SOURCE_DIR}/bin/archive")
    message(STATUS "Now is Linux system")
endif()

# bin; /bin/library; /bin/archives
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${trage_runtime_output_dir}")         # .exe 或可执行文件（应用程序） -> exe 和 dll 运行的输出路径
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${trage_library_output_dir}")         # 共享库（动态链接库）的 .dll .so(linux的输出目录)  ->    动态链接库（DLL、SO 等）的输出目录(实际不生效)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${trage_archive_output_dir}")         # 静态库（静态链接库）的 .lib .a(linux归档文件)静态库的输出目录

file(MAKE_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY} ${CMAKE_LIBRARY_OUTPUT_DIRECTORY} ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY})                      # 创建输出目录

# 实际上在不同系统 + 不同编译器上，编译后的产物的实际如下表效果
#----------------------------- Windows10 + MSVC 2022 -------------------------------------------
# CMAKE_RUNTIME_OUTPUT_DIRECTORY:  .exe + .dll   (Sunny.exe + WindowsRect.dll)
# CMAKE_LIBRARY_OUTPUT_DIRECTORY:  无
# CMAKE_ARCHIVE_OUTPUT_DIRECTORY:  .lib + .exp   (WindowsRect.exp + WindowsRect.lib + 若有则 qhotkey.lib)
#
#----------------------------- Windows 10 + MinGW 8.1.0 -------------------------------------------
# CMAKE_RUNTIME_OUTPUT_DIRECTORY:  .exe + .dll   (Sunny.exe + libWindowsRect.dll)
# CMAKE_LIBRARY_OUTPUT_DIRECTORY:  无
# CMAKE_ARCHIVE_OUTPUT_DIRECTORY:  .a + .dll.a   (libWindowsRect.dll.a + 若有则 libqhotkey.a)
#
#----------------------------- Ubuntu 22.04 + GCC 11.3.0 -------------------------------------------
# CMAKE_RUNTIME_OUTPUT_DIRECTORY:  无后缀        (Sunny )
# CMAKE_LIBRARY_OUTPUT_DIRECTORY:  .so          (libWindowsRect.so)
# CMAKE_ARCHIVE_OUTPUT_DIRECTORY:  .a           (thie project empty + 若有则 libqhotkey.a)
#------------------------------------------------------------------------

#           动态库      静态库           # Ref： https://blog.csdn.net/liuzehn/article/details/78424972
# Windows    .dll       .lib
# Linux      .so        .a
# Mac        .dylib     .a             (.framework 即可能是动态库、也可能是静态库)

if(NOT CMAKE_DEBUG_POSTFIX)                     # 好像仅添加到 库文件 的后缀
    if(WIN32)
        set(CMAKE_DEBUG_POSTFIX _debug)
    else()
        set(CMAKE_DEBUG_POSTFIX _debug)
    endif()
endif()

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
    set(CMAKE_MAKE_PROGRAM "make -j${N}")
endif()

add_subdirectory(src)
add_subdirectory(windows_rect)
#add_subdirectory(xwidgets)

#add_subdirectory(common)
