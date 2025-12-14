@echo off

::delete build dir
if "%1"=="delete" goto _DEL

if not exist build md build

cd build
cmake -G "Visual Studio 16 2019" -A x64 ..
goto _END

:_DEL
rd build/s/q
goto _END

:_END