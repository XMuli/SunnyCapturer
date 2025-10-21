# SPDX-License-Identifier: MIT
# SPDX-FileCopyrightText: 2021-2024 XMuli
# SPDX-GitHub: https://github.com/SunnyScreenshot/FLIPPED
# SPDX-Author: XMuli <xmulitech@gmail.com>

cmake_minimum_required(VERSION 3.5)

set(project_name FLIPPED)
set(project_version_major 1)
set(project_version_minor 3)
set(project_version_path 2)
set(project_version ${project_version_major}.${project_version_minor}.${project_version_path})
project(${project_name} VERSION ${project_version} LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(EXECUTABLE_OUTPUT_PATH "${CMAKE_CURRENT_SOURCE_DIR}/../bin") # qtcreator + msvc 可执行文件的输出目录
set(CMAKE_BINARY_DIR "${EXECUTABLE_OUTPUT_PATH}")                # visual studio 2022 可执行的输出目录
file(MAKE_DIRECTORY ${CMAKE_BINARY_DIR})                         # 创建输出目录
set(_VERSION ${VERSION})
#message("--------------------->${QT_VERSION} ${QT_VERSION_MAJOR}  ${QT_VERSION_MINOR}  ${QT_VERSION_PATCH}")

# 作为宏传递给 C++ 代码
add_definitions(-DMY_PROJECT_NAME="${project_name}")
add_definitions(-DMY_BUILD_TIME="2024-06-01")
add_definitions(-DMY_VARIABLE="${project_version}")
add_compile_definitions(MY_DEBUG_DETAIL)  # 屏蔽则关闭详细调试日志信息
#add_compile_definitions(MY_SHIELD_CONFIG_JSON)  # 屏蔽配置文件读写 json

set(qt_moduls Widgets Xml Svg)   # Multimedia Network WebSockets
set(qt_version_moduls)
foreach(it ${qt_moduls})
    list(APPEND qt_version_moduls Qt${QT_VERSION_MAJOR}::${it})
    message("--->qt_moduls:" Qt${QT_VERSION_MAJOR}::${it})
endforeach()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS ${qt_moduls} LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS ${qt_moduls} LinguistTools) # LinguistTools No link

set(PROJECT_WIDGETS
    widgets/chatinputedit.h
    widgets/chatinputedit.cpp
    widgets/bubble.h
    widgets/bubble.cpp
    widgets/bubble.ui
    widgets/bubblehistory.h
    widgets/bubblehistory.cpp
    widgets/aboutinfo.h
    widgets/aboutinfo.cpp
    widgets/aboutinfo.ui
)

set(SRC_MAIN
    xglobal.h
    main.cpp)

set(SRC_CORE
    core/isingleton.h
    core/xlog.h
    core/xqtlog.h
    core/xqtlog.cpp
    core/arrowline.h
    core/arrowline.cpp)

set(SRC_PLATFORM
    platform/iwininfo.h
    platform/iwininfo.cpp
    platform/wininfo.h
    platform/wininfo.cpp)

if(APPLE)
elseif(WIN32)
    list(APPEND SRC_PLATFORM
        platform/wininfo_win.h
        platform/wininfo_win.cpp)
else()
    list(APPEND SRC_PLATFORM
        platform/wininfo_x11.h
        platform/wininfo_x11.cpp)
endif()

set(SRC_WIDGET
    widget/xhorizontalline.h
    widget/xhorizontalline.cpp
    widget/xverticalline.h
    widget/xverticalline.cpp
    widget/xkeysequenceedit.h
    widget/xkeysequenceedit.cpp
    widget/xroundwidget.h
    widget/xroundwidget.cpp
    widget/xtextwidget.h
    widget/xtextwidget.cpp
    widget/xlabel.h
    widget/xlabel.cpp
    widget/xframewidget.h
    widget/xframewidget.cpp)

set(SRC_TOOL
    tool/base/colorparabar.h
    tool/base/colorparabar.cpp
    tool/base/managebar.h
    tool/base/managebar.cpp
    tool/base/blurwidget.h
    tool/base/blurwidget.cpp
    tool/selectsize/selectsize.h
    tool/selectsize/selectsize.cpp
    tool/magnifier/magnifier.h
    tool/magnifier/magnifier.cpp
    tool/pin/pinwidget.h
    tool/pin/pinwidget.cpp
    tool/selectbar.h
    tool/selectbar.cpp
    tool/parameterbar.h
    tool/parameterbar.cpp)

set(SRC_PERFERENCE
    preference/prefmanage.ui

    preference/prefmanage.h
    preference/prefmanage.cpp
#    preference/preference.h
#    preference/preference.cpp
    preference/hotkeyswidget.h
    preference/hotkeyswidget.cpp
    preference/appellation.h
    preference/appellation.cpp)

set(SRC_WINSCREEN
    screen/drawhelper.h
    screen/drawhelper.cpp
    screen/datamaid.h
    screen/datamaid.cpp
    screen/rectcalcu.h
    screen/rectcalcu.cpp
    screen/screenshot.h
    screen/screenshot.cpp
    screen/tray.h
    screen/tray.cpp)

set(SRC_PLUGIN_INTERFACE
    pluginsinterface/iplugininterface.h
    pluginsinterface/iplugininterface.cpp)

set(SRC_RESOURCES
    resources.qrc
    resources/logo/resources.rc)

set(src_root_path ${CMAKE_CURRENT_SOURCE_DIR})                                              # default root path curr path （CMakeList.txt）
file(GLOB_RECURSE source_lists LIST_DIRECTORIES false
    "${src_root_path}/*.cpp"
    "${src_root_path}/*.h")

source_group(TREE ${src_root_path} FILES ${source_lists})                                   # will be organized according to the actual directory structure, .h.cpp is put together
include_directories(${PROJECT_SOURCE_DIR}/widgets)  # 修复 Qt Design 使用提升自定义控件后， MinGW 找不到而编译失败

#********************************************** 判断且定义 x64/x86 变量 (get kit architecture bit) ****************************
if(CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(arch_bit "86")
    set(is_x64bit false)
elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(arch_bit "64")
    set(is_x64bit true)
endif()

# 定义一些 qt bin 的基础变量

set(qt_binray_dir "${Qt5_DIR}/../../../bin")   # C:/Qt/5.15.2/msvc2019_64/lib/cmake/Qt5/../../../bin

if(UNIX)  # using apt install qt5-devtools*
    execute_process(COMMAND lsb_release -i OUTPUT_VARIABLE LSB_ID)
    if (LSB_ID MATCHES "Deepin" OR LSB_ID MATCHES "UOS")
        set(lupdate_path "/usr/lib/qt5/bin/lupdate")
        set(lrelease_path "/usr/lib/qt5/bin/lrelease")
        message("This is a Deepin or UOS system.")
    else()
        set(lupdate_path "lupdate")
        set(lrelease_path "lrelease")
        message("This is other linux system.")
    endif()
else()
    set(lupdate_path "${qt_binray_dir}/lupdate")
    set(lrelease_path "${qt_binray_dir}/lrelease")

    if(WIN32)
        set(windeployqt "${qt_binray_dir}/windeployqt.exe")
    elseif(APPLE)
        set(windeployqt "${qt_binray_dir}/macdeployqt")
    elseif(UNIX)
        # custom deployqt
    endif()
endif()

message("Qt5_DIR:" ${Qt5_DIR})
message("qt_binray_dir:" ${qt_binray_dir})
message("windeployqt:" ${windeployqt})
message("lupdate_path:" ${lupdate_path})
message("lrelease_path:" ${lrelease_path})
message("EXECUTABLE_OUTPUT_PATH:" ${EXECUTABLE_OUTPUT_PATH})
message("CMAKE_BINARY_DIR:" ${CMAKE_BINARY_DIR})
message("CMAKE_SOURCE_DIR: ${CMAKE_SOURCE_DIR}")
message("CMAKE_RUNTIME_OUTPUT_DIRECTORY: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
message("CMAKE_RUNTIME_OUTPUT_DIRECTORY: ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")         # exe 和 dll 运行的输出路径
message("CMAKE_LIBRARY_OUTPUT_DIRECTORY: ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")         # 动态链接库（DLL、SO 等）的输出目录(实际不生效)
message("CMAKE_ARCHIVE_OUTPUT_DIRECTORY： ${CMAKE_ARCHIVE_OUTPUT_DIRECTORY}")         # 静态库的输出目录

#***************************************************通用参数的定义 开始**********************************************
# get kit architecture bit
if(CMAKE_SIZEOF_VOID_P EQUAL 4)
    set(architecture_bit "x86")
    set(build_x64bit false)
elseif(CMAKE_SIZEOF_VOID_P EQUAL 8)
    set(architecture_bit "x64")
    set(build_x64bit true)
endif()

if(APPLE)
    set(exe_dir ${EXECUTABLE_OUTPUT_PATH}/${project_name}.app/Contents/MacOS)
else()
    set(exe_dir ${EXECUTABLE_OUTPUT_PATH})
endif()

message("--->architecture_bit:" ${architecture_bit}  "  build_x64bit:" ${build_x64bit}  "  ssl_dir:" ${ssl_dir} "  HOME_DIR:" ${HOME_DIR} "  env_os:" ${env_os})
message("lupdate_path:" ${lupdate_path})
message("lrelease_path:" ${lrelease_path})
message("exe_dir:" ${exe_dir})
message("EXECUTABLE_OUTPUT_PATH:" ${EXECUTABLE_OUTPUT_PATH})
message("CMAKE_BINARY_DIR:" ${CMAKE_BINARY_DIR})

#***************************************************拷贝资源文件到 bin 下**********************************************
# 拷贝配置文件
file(GLOB all_config_files "src/resources/config/*")
file(COPY ${all_config_files} DESTINATION "${CMAKE_BINARY_DIR}/config")

# copy "resources" 文件夹下的指定文件夹
set(folders_to_copy "logo" "licenses" "changelog")
set(source_root "${CMAKE_SOURCE_DIR}/src/resources")
set(traget_root "${CMAKE_BINARY_DIR}/resources")

# 遍历复制文件夹集合中的每个文件夹
foreach(it ${folders_to_copy})
    set(copy_source_dir "${source_root}/${it}")
#    set(copy_traget_dir "${traget_root}/${it}")
    file(COPY ${copy_source_dir} DESTINATION ${traget_root})
endforeach()

#************************************************* .ts 生成 .qm***************************************************
# 创建保存 ts 的文件夹，和依据里面的 *.ts 文件，生成 .qm 放入最终文件中
set(ts_dir "${CMAKE_CURRENT_SOURCE_DIR}/translations")
if(APPLE)
    set(qm_dir ${EXECUTABLE_OUTPUT_PATH}/${project_name}.app/Contents/MacOS/translations)
else()
    set(qm_dir ${EXECUTABLE_OUTPUT_PATH}/translations)
endif()

file(MAKE_DIRECTORY "${ts_dir}")
file(MAKE_DIRECTORY "${qm_dir}")
file(GLOB ts_files "${ts_dir}/*.ts")

set(ts_files_name "")    # 获取文件名部分（不包括路径）
foreach(it ${ts_files})
    get_filename_component(ts_filename ${it} NAME_WE)
    list(APPEND ts_files_name ${ts_filename})
endforeach()

##编译时更新 .ts 和生成 .qm
foreach(it ${ts_files_name})
    # 命令参数问题：确保命令的参数传递方式正确。有时候在使用 execute_process 时，参数的传递需要一些特殊的格式，比如将参数放到列表中传递。
    execute_process(COMMAND "${lupdate_path}" -no-obsolete -no-ui-lines "${CMAKE_SOURCE_DIR}" -ts "${ts_dir}/${it}.ts") # 注意参数  # -no-obsolete -no-ui-lines
    execute_process(COMMAND "${lrelease_path}" "${ts_dir}/${it}.ts" -qm "${qm_dir}/${it}.qm")
endforeach()

file(GLOB_RECURSE qm_files "${qm_dir}/*.qm")

message("-->ts_files_name: ${ts_files_name}")
message("-->ts_files: ${ts_files}")
message("-->qm_files: ${qm_files}")
#***************************************************使用第三方库 QHotkey 开始**********************************************

include_directories(${CMAKE_SOURCE_DIR}/3rdparty/spdlog/include
    ${CMAKE_SOURCE_DIR}/3rdparty/QHotkey/QHotkey)
link_directories(${CMAKE_BINARY_DIR}/bin
    ${CMAKE_BINARY_DIR}/3rdparty/QHotkey)
#***************************************************使用第三方库 结束**********************************************
add_executable(${project_name}
    ${SRC_MAIN}
    ${SRC_PLATFORM}
    ${SRC_CORE}
    ${SRC_WIDGET}
    ${SRC_TOOL}
    ${SRC_PERFERENCE}
    ${SRC_WINSCREEN}
    ${SRC_EXAMPLE}
    ${SRC_PLUGIN_INTERFACE}
    ${SRC_RESOURCES}
    ${qm_files}  # Note: https://www.cnblogs.com/Paoyao/p/15752116.html
)

if (UNIX)
    target_link_libraries(${project_name} PRIVATE ${qt_version_moduls} qhotkey X11)
else()
    target_link_libraries(${project_name} PRIVATE ${qt_version_moduls} qhotkey)
endif()

set_target_properties(${project_name} PROPERTIES
    MACOSX_BUNDLE_GUI_IDENTIFIER https://thinkymate.com
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

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

if(MSVC)
    # 设置默认启动 project
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${project_name})
    # Fix: Visual Studio 2022 生成的目标文件的输出目录, 移除掉 Release/Debug 的多一层文件夹
    set_target_properties(${project_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_BINARY_DIR}")
    set_target_properties(${project_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_BINARY_DIR}")
    set_target_properties(${project_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_MINSIZEREL "${CMAKE_BINARY_DIR}")
    set_target_properties(${project_name} PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELWITHDEBINFO "${CMAKE_BINARY_DIR}")
    # 开启 MSVC 终端上的中文不乱码
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /utf-8")
    add_compile_options(/source-charset:utf-8 /execution-charset:utf-8)
    # 开启 UNICODE 等常规
    target_compile_definitions(${project_name} PRIVATE
        WIN32_LEAN_AND_MEAN                                                                  # Header files containing only the common APIs
        _CRT_SECURE_NO_WARNINGS                                                              # Unsafe functions such as strcpy, scanf, etc. are allowed
        UNICODE
        _UNICODE)
elseif(MINGW)
endif()

## Define some variables that can be used in *.cpp
#target_compile_definitions(${project_name} PUBLIC _BIT_ARCH=${CMAKE_SIZEOF_VOID_P})          # 4/8
#target_compile_definitions(${project_name} PUBLIC _COMPILER=${CMAKE_HOST_SYSTEM_PROCESSOR})  # i386/i686/x86_64/unknown  Win: AMD64
#target_compile_definitions(${project_name} PUBLIC _COMPILER_ID="${CMAKE_CXX_COMPILER_ID}")   # Clang/GCC/MSVC or GNU

# Define some variables that can be used in *.cpp
target_compile_definitions(${PROJECT_NAME} PUBLIC _PROJECT_NAME="${PROJECT_NAME}")
target_compile_definitions(${PROJECT_NAME} PUBLIC _PROJECT_VERSION="${PROJECT_VERSION}")
target_compile_definitions(${PROJECT_NAME} PUBLIC _BIT_ARCH=${CMAKE_SIZEOF_VOID_P})          # 4/8
target_compile_definitions(${PROJECT_NAME} PUBLIC _COMPILER=${CMAKE_HOST_SYSTEM_PROCESSOR})  # i386/i686/x86_64/unknown  Win: AMD64
target_compile_definitions(${PROJECT_NAME} PUBLIC _COMPILER_ID="${CMAKE_CXX_COMPILER_ID}")   # Clang/GCC/MSVC
# build time about version

message("------------BRGIN------------")
message("_PROJECT_NAME:${project_name}")
message("_PROJECT_VERSION:" ${project_version})
message("_BIT_ARCH:" ${CMAKE_SIZEOF_VOID_P})
message("_COMPILER:" ${CMAKE_HOST_SYSTEM_PROCESSOR})
message("_COMPILER_ID:" ${CMAKE_CXX_COMPILER_ID})
message("CMake Path:"
    "\n PROJECT_NAME:"  ${PROJECT_NAME}
    "\n CMAKE_INSTALL_PREFIX: "  ${CMAKE_INSTALL_PREFIX}
    "\n PROJECT_BINARY_DIR: "  ${PROJECT_BINARY_DIR}
    "\n CMAKE_INSTALL_BINDIR: "  ${CMAKE_INSTALL_BINDIR}
    "\n CMAKE_INSTALL_LIBDIR: "  ${CMAKE_INSTALL_LIBDIR}
    "\n CMAKE_INSTALL_INCLUDEDIR: "  ${CMAKE_INSTALL_INCLUDEDIR})
message("------------END------------")

install(TARGETS ${project_name}
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${project_name})
endif()

#打包和压缩***************************************************************************************
# 拷贝 ssl 和对应的 qt 依赖

if(APPLE)
    add_custom_command(TARGET ${project_name} POST_BUILD
        COMMAND "${windeployqt} ${EXECUTABLE_OUTPUT_PATH}/${project_name}.app" -verbose=1
    )

    add_custom_command(TARGET ${project_name} POST_BUILD
        COMMAND "${windeployqt}/ ${EXECUTABLE_OUTPUT_PATH}/${project_name}.app -dmg"
    )

elseif(UNIX)

elseif(WIN32)
    add_custom_command(TARGET ${project_name} POST_BUILD
        COMMAND "${windeployqt}" "${EXECUTABLE_OUTPUT_PATH}/${project_name}.exe" --no-translations
    )
endif()

message("--->windeployqt(or mac/linux): ${windeployqt}")

# 添加一个自定义的目标，将编译和自定义打包命令关联起来
add_custom_target(run_custom_qt_packaging
    DEPENDS ${project_name}
)


