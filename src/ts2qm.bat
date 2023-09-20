@echo off

set "PROJECT_NAME=Sunny"
set "QT_DIR=C:\Qt\5.15.2\msvc2019_64\bin"
set "SOURCE_DIR=./"
set "OUTPUT_TS_DIR=./translations"
set "OUTPUT_QM_DIR=../bin/translations"

set "LANGUAGES=en_US zh_CN zh_TW"

if not exist "%OUTPUT_QM_DIR%" (
    mkdir "%OUTPUT_QM_DIR%"
)

for %%a in (%LANGUAGES%) do (
    echo 执行 lupdate 命令: %QT_DIR%\lupdate.exe -no-obsolete %SOURCE_DIR% -ts "%OUTPUT_TS_DIR%\%PROJECT_NAME%_%%a.ts"
    %QT_DIR%\lupdate.exe -locations none -no-ui-lines %SOURCE_DIR% -ts "%OUTPUT_TS_DIR%\%PROJECT_NAME%_%%a.ts"
    
    echo 执行 lrelease 命令: %QT_DIR%\lrelease.exe "%OUTPUT_TS_DIR%\%PROJECT_NAME%_%%a.ts" -qm "%OUTPUT_QM_DIR%\%PROJECT_NAME%_%%a.qm"
    %QT_DIR%\lrelease.exe "%OUTPUT_TS_DIR%\%PROJECT_NAME%_%%a.ts" -qm "%OUTPUT_QM_DIR%\%PROJECT_NAME%_%%a.qm"
)

echo 脚本执行完毕
