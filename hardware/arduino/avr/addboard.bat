pushd %~dp0
echo off
if not exist boards.txt goto notfound
find /c "iprotoxi.name=iProtoXi Micro" boards.txt
if %errorlevel% equ 1 goto addboard
echo iProtoXi Micro already found
goto done
:addboard
if not exist microboard.txt goto notfound
type microboard.txt >> boards.txt
if %errorlevel% equ 1 goto requireadmin
echo iProtoXi Micro succesfully added
goto done
:notfound
echo boards.txt or microboard.txt not found, are you sure you are in the right folder
goto done
:requireadmin
echo Admin privileges required, please run script as administrator
goto done
:done
pause