@echo off

set "QT_DIR=C:/Qt/5.15.2/msvc2019_64/bin"
set "SOURCE_DIR=./"
set "OUTPUT_TS_DIR=./translations"
set "OUTPUT_QM_DIR=../bin/translations"

rem 设置想要的语言列表 https://www.afforange.com/556.html
rem set "LANGUAGES=en_us en_gb zh_cn zh_tw zh_sg es ar fr ru pt pt_br de ja it ko pl nl tr id uk th sv fi cs hi el he no da hu vi ro sr bg"
set "LANGUAGES=en_us zh_cn zh_tw pt_br"

if not exist "%OUTPUT_QM_DIR%" (
    mkdir "%OUTPUT_QM_DIR%"
)

rem 初始化计数器
set "TOTAL_UPDATED=0"
set "SUCCESS_COUNT=0"
set "FAILURE_COUNT=0"

rem 遍历语言列表
for %%a in (%LANGUAGES%) do (
    rem 执行 lupdate 命令
    echo 执行 lupdate 命令: %QT_DIR%/lupdate.exe -locations none -no-obsolete %SOURCE_DIR% -ts "%OUTPUT_TS_DIR%/%%a.ts"
    %QT_DIR%\lupdate.exe -locations none -no-obsolete %SOURCE_DIR% -ts "%OUTPUT_TS_DIR%/%%a.ts"
    
    rem 执行 lrelease 命令
    echo 执行 lrelease 命令: %QT_DIR%/lrelease.exe "%OUTPUT_TS_DIR%/%%a.ts" -qm "%OUTPUT_QM_DIR%/%%a.qm"
    %QT_DIR%\lrelease.exe "%OUTPUT_TS_DIR%/%%a.ts" -qm "%OUTPUT_QM_DIR%/%%a.qm"

    rem 更新计数器
    set /A "TOTAL_UPDATED+=1"
    if %ERRORLEVEL% EQU 0 (
        set /A "SUCCESS_COUNT+=1"
    ) else (
        set /A "FAILURE_COUNT+=1"
    )
)

rem 输出统计结果
echo 脚本执行完毕
echo 更新了 %TOTAL_UPDATED% 个文件。
echo 成功更新了 %SUCCESS_COUNT% 个文件。
echo 更新失败了 %FAILURE_COUNT% 个文件。
