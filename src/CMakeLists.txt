cmake_minimum_required(VERSION 3.5)

set(project_name Sunny)
set(project_version_major 1)
set(project_version_minor 1)
set(project_version_path 0)
set(project_version ${project_version_major}.${project_version_minor}.${project_version_path})
project(${project_name} VERSION ${project_version} LANGUAGES CXX)

add_definitions(-DXBUILD_TIME="2023.10.29")


set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 各种调试开关
#add_compile_definitions(HALF_SCRN_DEVELOP)           # 便于调试，多屏幕-仅一个副屏幕显示; 单屏-版屏幕效果，版屏幕效果(开启)
#add_compile_definitions(HIDE_TOOL_BAR_BLUR_EFFECT)   # 开启-隐藏 PaintToolBar 的跨平台模糊效果
#add_compile_definitions(LOG_MORE_INFO_DEVELOP)       # 便于调试，打印更多的信息，包含文件名，行数函数名 等信息到日志里面
#add_compile_definitions(LOG_CURR_POS_IN_RECTS)       # 便于调试, 当前光标所在的所有 窗口矩形 的信息
add_compile_definitions(QT_MESSAGELOGCONTEXT)         # Release 日志也可以输出 文件名 和 路径

set(qt_moduls Widgets Xml Svg)                        # Network Multimedia WebSockets
set(qt_version_moduls)
foreach(it ${qt_moduls})
    list(APPEND qt_version_moduls Qt${QT_VERSION_MAJOR}::${it})
    message("--->qt_moduls:" Qt${QT_VERSION_MAJOR}::${it})
endforeach()

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS ${qt_moduls} LinguistTools)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS ${qt_moduls} LinguistTools) # LinguistTools No link

set(commom_files
    commom/include/xlog.h
    commom/include/isingleton.h
    commom/include/xapphelper.h

    commom/sources/xlog.cpp
    commom/sources/xapphelper.cpp

    data/config_global.h
    data/configmanager.h
    data/configmanager.cpp
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
    ui/include/xtoolbutton.h
    # h composite_widgets
    ui/include/xblureffect.h
    ui/include/colorpicker.h
    ui/include/resetui.h
    ui/include/general.h
    ui/include/interface.h
    ui/include/output.h
    ui/include/pin.h
    ui/include/hotkeys.h
    ui/include/about.h
    ui/include/licenseui.h
    #  cpp common_widgets
    ui/common_widgets/xblureffect.cpp
    ui/common_widgets/xcolorbutton.cpp
    ui/common_widgets/xkeysequenceedit.cpp
    ui/common_widgets/horspacerline.cpp
    ui/common_widgets/verspacerline.cpp
    ui/common_widgets/tips.cpp
    ui/common_widgets/xtextedit.cpp
    ui/common_widgets/xtoolbutton.cpp
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
    ui/composite_widgets/pin.cpp
    ui/composite_widgets/pin.ui
    ui/composite_widgets/licenseui.cpp
    ui/composite_widgets/licenseui.ui

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
    ui/paint_bar/pin/pinwidget.ui

    resources.qrc
    resources/logo/resources.rc
)

set(src_root_path ${CMAKE_CURRENT_SOURCE_DIR})                                                 # default root path curr path （CMakeList.txt）
file(GLOB_RECURSE source_lists LIST_DIRECTORIES false "${src_root_path}/*.cpp" "${src_root_path}/*.h")
source_group(TREE ${src_root_path} FILES ${source_lists})
include_directories("${CMAKE_CURRENT_SOURCE_DIR}/ui/include")                                    # 修复 Qt Design 使用提升自定义控件后， MinGW 找不到而编译失败; 且 MSVC 和 MinGW 使用提升时候，都不需要加上 相对路径
include_directories("${CMAKE_SOURCE_DIR}/tripartite/QHotkey")


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
    set(lupdate_path "lupdate")
    set(lrelease_path "lrelease")
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
#************************************************* 拷贝 .ini 和资源文件 *******************************************************
# copy "resources" 文件夹下的指定文件夹
set(folders_to_copy "changelog" "licenses" "logo" "linux")
set(source_root "${CMAKE_CURRENT_SOURCE_DIR}/resources")
set(traget_root "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources")

# 遍历复制文件夹集合中的每个文件夹
foreach(it ${folders_to_copy})
    set(copy_source_dir "${source_root}/${it}")
    file(COPY ${copy_source_dir} DESTINATION ${traget_root})
endforeach()

file(COPY "${CMAKE_SOURCE_DIR}/xconfig.ini" DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")

#if(UNIX)
#    file(GLOB sh_files "${source_root}/linux/debian/*.sh")
#    file(COPY ${sh_files} DESTINATION ${CMAKE_RUNTIME_OUTPUT_DIRECTORY})
#endif()
#************************************************* update .ts, and generate .qm *******************************************************
set(ts_dir "${CMAKE_CURRENT_SOURCE_DIR}/translations")
if(APPLE)
    set(qm_dir ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${project_name}.app/Contents/MacOS/translations)
else()
    set(qm_dir ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/translations)
endif()

file(MAKE_DIRECTORY "${ts_dir}" "${qm_dir}")
file(GLOB ts_files "${ts_dir}/*.ts")

set(ts_files_name "")    # 获取文件名部分（不包括路径）
foreach(it ${ts_files})
    get_filename_component(ts_filename ${it} NAME_WE)
    list(APPEND ts_files_name ${ts_filename})
endforeach()

##编译时更新 .ts 和生成 .qm, # 命令参数问题：确保命令的参数传递方式正确。有时候在使用 execute_process 时，参数的传递需要一些特殊的格式，比如将参数放到列表中传递。
foreach(it ${ts_files_name})
    execute_process(COMMAND "${lupdate_path}" -locations none -no-obsolete -no-ui-lines "${CMAKE_CURRENT_SOURCE_DIR}" -ts "${ts_dir}/${it}.ts") # 注意参数  # -no-obsolete -no-ui-lines
    execute_process(COMMAND "${lrelease_path}" "${ts_dir}/${it}.ts" -qm "${qm_dir}/${it}.qm")
endforeach()

file(GLOB_RECURSE qm_files "${qm_dir}/*.qm")

message("-->ts_files_name: ${ts_files_name}")
message("-->ts_files: ${ts_files}")
message("-->qm_files: ${qm_files}")
#******************************************* 调用 WindowsRect DLL 共 ①~④ 步骤 **********************************************
include_directories(${CMAKE_CURRENT_SOURCE_DIR}/windows_rect/include/)                           # ①设置头文件目录
link_directories(${trage_runtime_output_dir})                                                    # ②添加链接库路径
#***************************************************************************************************************************

add_executable(${project_name}
    ${commom_files}
    ${src_files}
    ${custom_ui_files}
    ${qm_files}
)

# Define some variables that can be used in *.cpp
target_compile_definitions(${project_name} PRIVATE XPROJECT_NAME="${project_name}")
target_compile_definitions(${project_name} PRIVATE XPROJECT_VERSION="${project_version}")
target_compile_definitions(${project_name} PRIVATE XARCH_BIT="${arch_bit}")
target_compile_definitions(${project_name} PRIVATE XCOMPILER="${CMAKE_HOST_SYSTEM_PROCESSOR}")  # i386/i686/x86_64/unknown  Win: AMD64
target_compile_definitions(${project_name} PRIVATE XCOMPILER_ID="${CMAKE_CXX_COMPILER_ID}")     # Clang/GCC/MSVC or GNU

message("------------BRGIN------------")
message("XPROJECT_NAME:${project_name}")
message("XPROJECT_VERSION:${project_version}")
message("XARCH_BIT:${arch_bit}")
message("XCOMPILER:${CMAKE_HOST_SYSTEM_PROCESSOR}")
message("XCOMPILER_ID:${CMAKE_CXX_COMPILER_ID}")

message("CMake Path:"
    "\nPROJECT_NAME:"  ${project_name}
    "\nCMAKE_INSTALL_PREFIX:"  ${CMAKE_INSTALL_PREFIX}
    "\nPROJECT_BINARY_DIR:"    ${PROJECT_BINARY_DIR}
    "\nCMAKE_INSTALL_BINDIR:"  ${CMAKE_INSTALL_BINDIR}
    "\nCMAKE_INSTALL_LIBDIR:"  ${CMAKE_INSTALL_LIBDIR}
    "\nCMAKE_INSTALL_INCLUDEDIR:"  ${CMAKE_INSTALL_INCLUDEDIR})
message("------------END------------")


# 依据不同 build type, 生成实际 link 的 real dll 名
set(dependent_dll_name WindowsRect)  # xwidgets
set(dependent_dll_real_name)
foreach(it ${dependent_dll_name})
    if(CMAKE_BUILD_TYPE STREQUAL "Debug")
        set(it ${it}${CMAKE_DEBUG_POSTFIX})
    endif()
    message("--> curr dll {it} name:" "${it}")
    list(APPEND dependent_dll_real_name "${it}")
endforeach()

message("--> CMAKE_DEBUG_POSTFIX:${CMAKE_DEBUG_POSTFIX}  dependent_dll_name:${dependent_dll_name} dependent_dll_real_name:${dependent_dll_real_name}")
message("---@2--> ${CMAKE_CURRENT_SOURCE_DIR}/../windows_rect/include")
message("---@3--> ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}")

add_dependencies(${project_name} ${dependent_dll_name})                                         # ③添加依赖顺序，确保先构建依赖，此处是 工程名称，而非 dll 名称(没有后缀d)
target_link_directories(${project_name} PRIVATE ${CMAKE_LIBRARY_OUTPUT_DIRECTORY})
target_link_libraries(${project_name} PRIVATE ${qt_version_moduls} ${dependent_dll_real_name} QHotkey::QHotkey)  # ④链接 dll 库； 同时需要 .lib 和 .dll 在同一个路径
target_include_directories(${project_name} PRIVATE                                              # .cpp 文件可以直接使用 include "文件名.h"，去掉相对路径
    ${CMAKE_CURRENT_SOURCE_DIR}/commom/include
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/include
    ${CMAKE_CURRENT_SOURCE_DIR}/ui/commom/communication
    ${CMAKE_CURRENT_SOURCE_DIR}/../windows_rect/include
)
target_compile_definitions(${project_name} PRIVATE XWIDGET_LIBRARY UNICODE _UNICODE)                         # 定义 XWIDGET_LIBRARY 宏( ET 后面没有 S)，表明为导出。后期将导出为 动态库 封装

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
        _CRT_SECURE_NO_WARNINGS                                                              # Unsafe functions such as strcpy, scanf, etc. are allowed)
    )
endif()


#******************************************* 拷贝 ssl 和对应的 qt 依赖 *******************************************************
if(APPLE)
#    file(COPY "/usr/local/Cellar/openssl@3/3.1.1/lib/libcrypto.3.dylib" DESTINATION "${EXECUTABLE_OUTPUT_PATH}")
#    file(COPY "/usr/local/Cellar/openssl@3/3.1.1/lib/libssl.3.dylib" DESTINATION "${exe_output_dir}")
#    add_custom_command(TARGET ${project_name} POST_BUILD COMMAND "${windeployqt} ${EXECUTABLE_OUTPUT_PATH}/${project_name}.app" -verbose=1 )
#    add_custom_command(TARGET ${project_name} POST_BUILD COMMAND "${windeployqt}/ ${EXECUTABLE_OUTPUT_PATH}/${project_name}.app -dmg" )

elseif(UNIX)

elseif(WIN32)

#    if(${is_x64bit})
#        set(ssl_dir "C:/Qt/Tools/OpenSSL/Win_x64/")
#        set(ssl_file "libssl-1_1-x64.dll" "libcrypto-1_1-x64.dll")
#    else()
#        set(ssl_dir "C:/Qt/Tools/OpenSSL/Win_x86/")
#        set(ssl_file "libssl-1_1.dll" "libcrypto-1_1.dll")
#    endif()

#    foreach(it ${ssl_file})
#        file(COPY "${ssl_dir}/bin/${it}" DESTINATION "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}")
#        message("--->it:" "${ssl_dir}/bin/${it}")
#    endforeach()

    add_custom_command(TARGET ${project_name} POST_BUILD COMMAND "${windeployqt}" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/${project_name}.exe" --no-opengl-sw --no-translations)
endif()
#****************************************************** 稚拙 Ununtu 22.04 的 .deb *********************************************************************

include(GNUInstallDirs)
include (FeatureSummary)  # using  set_package_properties

message("CMAKE_INSTALL_DATADIR: ${CMAKE_INSTALL_DATADIR}")

if (WIN32)
elseif (APPLE)
elseif (UNIX)
    # Helper macros for parsing and setting project version from `git describe --long` result
    macro (ppic_set_version_via_describe _describe_long)
        string (
            REGEX REPLACE
            "^([0-9a-z.]*)-[0-9]+-g[0-9a-f]*$"
            "\\1"
            _tag_parts
            "${_describe_long}"
        )
        list (GET _tag_parts 0 _matched_tag_version)
        if ("${_matched_tag_version}" MATCHES "^[0-9]+\\.[0-9]+\\.[0-9]+$")
            string (
                REGEX REPLACE
                "^([0-9]+)\\.([0-9]+)\\.([0-9]+).*$"
                "\\1;\\2;\\3"
                _ver_parts
                "${_matched_tag_version}"
            )
            list (GET _ver_parts 0 CPACK_PACKAGE_VERSION_MAJOR)
            list (GET _ver_parts 1 CPACK_PACKAGE_VERSION_MINOR)
            list (GET _ver_parts 2 CPACK_PACKAGE_VERSION_PATCH)
        endif ()
    endmacro ()

    # Version setup
    if (EXISTS "${CMAKE_SOURCE_DIR}/.git")
        find_package(Git)
        set_package_properties(Git PROPERTIES TYPE OPTIONAL PURPOSE "Determine exact build version.")
        if (GIT_FOUND)
            execute_process (
                COMMAND ${GIT_EXECUTABLE} describe --tags --always --long
                WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
                OUTPUT_VARIABLE _git_describe_long
            )
            string (REGEX REPLACE "\n" "" _git_describe_long "${_git_describe_long}")
            ppic_set_version_via_describe(${_git_describe_long})
            target_compile_definitions(${project_name} PRIVATE
                GIT_DESCRIBE_VERSION_STRING="${_git_describe_long}"
            )
        endif ()
    endif ()

    set(CMAKE_INSTALL_PREFIX /usr)
    # [/usr/share] -------------------------
    # install /usr/share/applications/tech.xmuli.sunny.desktop
    install (FILES ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/linux/debian/tech.xmuli.sunny.desktop
             DESTINATION "${CMAKE_INSTALL_DATAROOTDIR}/applications"
             )
    # install /usr/share/icons/hicolor/scalable/apps/tech.xmuli.sunny.svg
    install (FILES ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/logo/logo.svg
             DESTINATION "${CMAKE_INSTALL_DATADIR}/icons/hicolor/scalable/apps"
             RENAME tech.xmuli.sunny.svg
             )
    # install /usr/share/metainfo/tech.xmuli.sunny.metainfo.xml
    install (FILES ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/linux/debian/appstream/tech.xmuli.sunny.metainfo.xml
             DESTINATION "${CMAKE_INSTALL_DATADIR}/metainfo"
             )
    # [/usr/local] -------------------------
    # install /usr/local/Sunny/xconfig.ini
    install (FILES ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/xconfig.ini
             DESTINATION "/usr/local/${project_name}"
             )
    # install /usr/local/Sunny/translations/Sunny_*.qm
    install (DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/translations
             DESTINATION "/usr/local/${project_name}"
             FILES_MATCHING PATTERN "*.qm"
             )
     # install /usr/local/Sunny/resources folder
    install (DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources
             DESTINATION "/usr/local/${project_name}"
             FILES_MATCHING PATTERN "*.*"
             )
    # [/usr/lib] -------------------------
    # install /usr/lib/libWindowsRect.so
    install (FILES ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/libWindowsRect.so
             DESTINATION ${CMAKE_INSTALL_LIBDIR}
             )
    # [/usr/bin] -------------------------
    # install /usr/bin/Sunny
    install (TARGETS ${project_name}
             RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
             LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
             ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR} COMPONENT Devel
             )

    message("---------->:" ${CMAKE_LIBRARY_OUTPUT_DIRECTORY}/*.so)
    message("---------->RUNTIME:" ${RUNTIME})
    message("---------->LIBRARY:" ${LIBRARY})
    message("---------->ARCHIVE:" ${ARCHIVE})
    message("---------->CMAKE_INSTALL_BINDIR:" ${CMAKE_INSTALL_BINDIR})
    message("---------->CMAKE_INSTALL_LIBDIR:" ${CMAKE_INSTALL_LIBDIR})
    message("---------->CMAKE_INSTALL_LIBDIR:" ${CMAKE_INSTALL_LIBDIR})
    message("---------->qm:" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/translations")

    feature_summary(WHAT ALL INCLUDE_QUIET_PACKAGES FATAL_ON_MISSING_REQUIRED_PACKAGES)

    # CPACK: General Settings
    set (CPACK_GENERATOR "TBZ2")
    set (CPACK_PACKAGE_NAME "${project_name}")
    set (CPACK_PACKAGE_VERSION "${project_version}")
    set (CPACK_PACKAGE_VENDOR "https://github.com/XMuli")
    set (CPACK_PACKAGE_DESCRIPTION_SUMMARY "Simple and beautiful screenshot software tool for Windows, MacOS and Linux")
    set (CPACK_PACKAGE_FILE_NAME "${CPACK_PACKAGE_NAME}-${CPACK_PACKAGE_VERSION}")
    set (CPACK_PACKAGE_CONTACT "https://sunny.xmuli.tech")
    # 设置Debian软件包的依赖关系
    set (CPACK_DEBIAN_PACKAGE_DEPENDS "libqt5x11extras5, libqt5svg5")
    set (CPACK_SYSTEM_NAME "${CMAKE_SYSTEM_NAME}-${CMAKE_SYSTEM_PROCESSOR}")
    set (CPACK_DEBIAN_PACKAGE_SHILIBDEPS ON)

    include(CPack)
endif()

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(${project_name})
endif()
