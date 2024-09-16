@echo off

pushd ..
vendor\premake5.exe --file=Build.lua vs2022
popd
pause