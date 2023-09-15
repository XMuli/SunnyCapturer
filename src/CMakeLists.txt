cmake_minimum_required(VERSION 3.5)

set(project_name Sunny)
set(project_version_major 1)
set(project_version_minor 0)
set(project_version_path 0)
set(project_version ${project_version_major}.${project_version_minor}.${project_version_path})
project(${project_name} VERSION ${project_version} LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 各种调试开关
add_compile_definitions(HALF_SCRN_DEVELOP)           # 便于调试，多屏幕-仅一个副屏幕显示; 单屏-版屏幕效果，版屏幕效果(开启)
#add_compile_definitions(HIDE_TOOL_BAR_BLUR_EFFECT)   # 开启-隐藏 PaintToolBar 的跨平台模糊效果
#add_compile_definitions(LOG_MORE_INFO_DEVELOP)       # 便于调试，打印更多的信息，包含文件名，行数函数名 等信息到日志里面
#add_compile_definitions(LOG_CURR_POS_IN_RECTS)       # 便于调试, 当前光标所在的所有 窗口矩形 的信息

set(qt_moduls Widgets Xml Svg)                        # Network Multimedia WebSockets
set(qt_version_moduls)
foreach(it ${qt_moduls})
    list(APPEND qt_version_moduls Qt${QT_VERSION_MAJOR}::${it})
    message("--->qt_moduls:" Qt${QT_VERSION_MAJOR}::${it})
endforeach()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS ${qt_moduls})
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS ${qt_moduls})

set(commom_files
    commom/include/xlog.h
    commom/include/isingleton.h

    commom/sources/xlog.cpp
)

set(custom_ui_files
    #  h common_widgets
    ui/include/xwidget_global.h
    ui/include/xcolorbutton.h
    ui/include/xkeysequenceedit.h
    ui/include/horspacerline.h
    ui/include/verspacerline.h
    ui/include/tips.h
    ui/include/xtextedit.h
    # h composite_widgets
    ui/include/xblureffect.h
    ui/include/colorpicker.h
    ui/include/resetui.h
    ui/include/general.h
    ui/include/interface.h
    ui/include/output.h
    ui/include/paste.h
    ui/include/hotkeys.h
    ui/include/about.h
    #  cpp common_widgets
    ui/common_widgets/xblureffect.cpp
    ui/common_widgets/xcolorbutton.cpp
    ui/common_widgets/xkeysequenceedit.cpp
    ui/common_widgets/horspacerline.cpp
    ui/common_widgets/verspacerline.cpp
    ui/common_widgets/tips.cpp
    ui/common_widgets/xtextedit.cpp
    #  cpp composite_widgets
    ui/composite_widgets/colorpicker.cpp
    ui/composite_widgets/resetui.cpp
    ui/composite_widgets/resetui.ui
    ui/composite_widgets/general.cpp
    ui/composite_widgets/general.ui
    ui/composite_widgets/interface.cpp
    ui/composite_widgets/interface.ui
    ui/composite_widgets/output.cpp
    ui/composite_widgets/output.ui
    ui/composite_widgets/hotkeys.cpp
    ui/composite_widgets/hotkeys.ui
    ui/composite_widgets/about.cpp
    ui/composite_widgets/about.ui
    ui/composite_widgets/paste.cpp
    ui/composite_widgets/paste.ui

    ui/paint_bar/common/absbtnsctrl.h
    ui/paint_bar/common/absbtnsctrl.cpp
    ui/paint_bar/toolbar_level/painttoolbar.h
    ui/paint_bar/toolbar_level/painttoolbar.cpp
    ui/paint_bar/toolbar_level/paintbarhelper.h
    ui/paint_bar/toolbar_level/paintbarhelper.cpp
    ui/paint_bar/toolbar_level/paintctrlbar.h
    ui/paint_bar/toolbar_level/paintctrlbar.cpp
    ui/paint_bar/paintbar.h
    ui/paint_bar/paintbar.cpp
)

set(src_files
    main.cpp
    ui/setting/sidesettingui.cpp
    ui/setting/sidesettingui.h
    ui/setting/sidesettingui.ui
    ui/setting/abssettingui.h
    ui/setting/abssettingui.cpp
    ui/setting/abssettingui.ui
    ui/screenshot/capturehelper.h
    ui/screenshot/capturehelper.cpp
    ui/screenshot/screenshot.h
    ui/screenshot/screenshot.cpp
    ui/screenshot/tray.h
    ui/screenshot/tray.cpp
    ui/commom/communication/communication.h
    ui/commom/communication/communication.cpp
    ui/paint_bar/pin/pinwidget.h
    ui/paint_bar/pin/pinwidget.cpp

    resources.qrc
)

set(src_root_path ${CMAKE_CURRENT_SOURCE_DIR})                                                 # default root path curr path （CMakeList.txt）
file(GLOB_RECURSE source_lists LIST_DIRECTORIES false "${src_root_path}/*.cpp" "${src_root_path}/*.h")
source_group(TREE ${src_root_path} FILES ${source_lists})
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/ui/include")                                    # 修复 Qt Design 使用提升自定义控件后， MinGW 找不到而编译失败; 且 MSVC 和 MinGW 使用提升时候，都不需要加上 相对路径
include_directories("${CMAKE_SOURCE_DIR}/tripartite/QHotkey")

#************************************************************************
#调用 WindowsRect DLL 共 ①~④ 步骤
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/windows_rect/include/)                           # ①设置头文件目录
link_directories(${trage_runtime_output_dir})                                                    # ②添加链接库路径
#************************************************************************

add_executable(${project_name}
    ${commom_files}
    ${src_files}
    ${custom_ui_files}
)


# Define some variables that can be used in *.cpp
target_compile_definitions(${project_name} PRIVATE XPROJECT_NAME=${project_name})
target_compile_definitions(${project_name} PRIVATE XPROJECT_VERSION=${project_version})
target_compile_definitions(${project_name} PRIVATE XBIT_ARCH=${CMAKE_SIZEOF_VOID_P})          # 4/8
target_compile_definitions(${project_name} PRIVATE XCOMPILER=${CMAKE_HOST_SYSTEM_PROCESSOR})  # i386/i686/x86_64/unknown  Win: AMD64
target_compile_definitions(${project_name} PRIVATE XCOMPILER_ID="${CMAKE_CXX_COMPILER_ID}")   # Clang/GCC/MSVC or GNU
#target_compile_definitions(${project_name} PRIVATE XIS_X64BIT=${is_x64bit})

message("------------BRGIN------------")
message("XPROJECT_NAME:${project_name}")
message("XPROJECT_VERSION:${project_version}")
message("XBIT_ARCH:${CMAKE_SIZEOF_VOID_P}")
message("XCOMPILER:${CMAKE_HOST_SYSTEM_PROCESSOR}")
message("XCOMPILER_ID:${CMAKE_CXX_COMPILER_ID}")
#message("XIS_X64BIT:" ${is_x64bit})

message("CMake Path:"
    "\nPROJECT_NAME:"  ${PROJECT_NAME}
    "\nCMAKE_INSTALL_PREFIX:"  ${CMAKE_INSTALL_PREFIX}
    "\nPROJECT_BINARY_DIR:"    ${PROJECT_BINARY_DIR}
    "\nCMAKE_INSTALL_BINDIR:"  ${CMAKE_INSTALL_BINDIR}
    "\nCMAKE_INSTALL_LIBDIR:"  ${CMAKE_INSTALL_LIBDIR}
    "\nCMAKE_INSTALL_INCLUDEDIR:"  ${CMAKE_INSTALL_INCLUDEDIR})
message("------------END------------")


# 依据不同 build type, 生成实际 link 的 real dll 名
set(dependent_dll_name WindowsRect)
set(dependent_dll_real_name ${dependent_dll_name})
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(dependent_dll_real_name "${dependent_dll_name}${CMAKE_DEBUG_POSTFIX}")
endif()
message("--> CMAKE_DEBUG_POSTFIX:${CMAKE_DEBUG_POSTFIX}  dependent_dll_name:${dependent_dll_name} dependent_dll_real_name:${dependent_dll_real_name}")
message("---@2--> ${CMAKE_CURRENT_SOURCE_DIR}/../windows_rect/include")

add_dependencies(${project_name} ${dependent_dll_name})                                         # ③添加依赖顺序，确保先构建依赖，此处是 工程名称，而非 dll 名称(没有后缀d)
target_link_libraries(${project_name} PRIVATE ${qt_version_moduls} ${dependent_dll_real_name} QHotkey::QHotkey)  # ④链接 dll 库； 同时需要 .lib 和 .dll 在同一个路径
target_include_directories(${project_name} PRIVATE                                              # .cpp 文件可以直接使用 include "文件名.h"，去掉相对路径
    ${CMAKE_CURRENT_SOURCE_DIR}/commom/include
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/include
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/commom/communication
    ${CMAKE_CURRENT_SOURCE_DIR}/../windows_rect/include
)
target_compile_definitions(${project_name} PRIVATE XWIDGET_LIBRARY)                         # 定义 XWIDGET_LIBRARY 宏( ET 后面没有 S)，表明为导出。后期将导出为 动态库 封装

set_target_properties(${project_name} PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

if(MSVC)
    # 设置默认启动 project
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${project_name})
    # Fix: Visual Studio 2022 生成的目标文件的输出目录, 移除掉 Release/Debug 的多一层文件夹    
    set_target_properties(${project_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${trage_runtime_output_dir}")
    set_target_properties(${project_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${trage_runtime_output_dir}")
    set_target_properties(${project_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${trage_runtime_output_dir}")
    set_target_properties(${project_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${trage_runtime_output_dir}")
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
        UNICODE
        _UNICODE)
endif()

include(GNUInstallDirs)
install(TARGETS ${project_name}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${project_name})
endif()
