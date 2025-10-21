@echo off

::delete build dir
if "%1"=="delete" goto _DEL

if not exist build md build

::"C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\VC\Auxiliary\Build\vcvarsall.bat" x64
cd build
cmake -G "Visual Studio 16 2019" -A x64 ..
::devenv Flipped.sln /Build "Release|x64"
::windeployqt --no-angle --no-opengl-sw bin/Flipped.exe
goto _END

:_DEL
rd build/s/q
goto _END

:_END