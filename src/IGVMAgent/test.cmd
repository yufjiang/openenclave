@echo off

echo "Setting up VS dev cmd. %~dp0"
call "C:\Program Files (x86)\Microsoft Visual Studio\2019\Enterprise\Common7\Tools\VsDevCmd.bat"

echo "Running IGVMAgent tests"
pushd "%~dp0"
pushd Debug
call "ctest" || exit \b 1
popd
pushd Release
call "ctest" || exit \b 1
popd
popd
echo "Done IGVMAgent tests"