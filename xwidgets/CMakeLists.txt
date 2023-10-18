cmake_minimum_required(VERSION 3.5)
#project(XWidgets)
option(BUILD_EXAMPLES "Build test demo applications." ON) # ON/OFF 切换后，需要删除 build 文件夹，再重新构建 CMake 即可看到 IDE 的工程变化

add_subdirectory(src)
if(BUILD_EXAMPLES)
    add_subdirectory(demo)
endif()
