cmake_minimum_required(VERSION 3.5)
#project(XWidgets)
option(BUILD_EXAMPLES "Build test demo applications." ON) # ON/OFF 切换后，需要删除 build 文件夹，再重新构建 CMake 即可看到 IDE 的工程变化

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_DEBUG_POSTFIX)
    if(WIN32)
        set(CMAKE_DEBUG_POSTFIX d)
    else()
        set(CMAKE_DEBUG_POSTFIX _debug)
    endif()
endif()

#           动态库      静态库           # Ref： https://blog.csdn.net/liuzehn/article/details/78424972
# Windows    .dll       .lib
# Linux      .so        .a
# Mac        .dylib     .a             (.framework 即可能是动态库、也可能是静态库)

add_subdirectory(src)
if(BUILD_EXAMPLES)
    add_subdirectory(demo)
endif()
