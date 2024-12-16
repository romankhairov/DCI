:: Copyright 2024 3D Source, Inc. All Rights Reserved.

@echo off

setlocal

set UEDir=%1
set UEDir=%UEDir:"=%
set DestDir=%2
set DestDir=%DestDir:"=%
set Guid=%3
set GenerateCollision=%4

set ThisDir=%~dp0
set UtilsDir=%ThisDir%..\Utils
set PakExportDir=%UtilsDir%\PakExport
set ProjectFilePath=%PakExportDir%\PakExport.uproject
set PakEDir=%PakExportDir%\Plugins\%Guid%
set PakESavedDir=%PakEDir%\Saved
set SourcePakPath=%PakESavedDir%\StagedBuilds\Windows\PakExport\Plugins\%Guid%\Content\Paks\Windows\%Guid%PakExport-Windows.pak

set UECmdDir=%UEDir%\Binaries\Win64\UnrealEditor-Cmd.exe

echo UEDir=%UEDir%
echo DestDir=%DestDir%
echo Guid=%Guid%
echo GenerateCollision=%GenerateCollision%

::Move assets
::call "%UEDir%\Build\BatchFiles\Build.bat" "PakExportEditor" Win64 Development "%ProjectFilePath%"

setlocal EnableDelayedExpansion

:while
    "%UECmdDir%" "%ProjectFilePath%" -IgnoreChangeList map=/Game/Minimal_Default.umap -UNATTENDED -RUNNINGUNATTENDEDSCRIPT -collision=True -Save -rebuild -nosourcecontrol -previewsurface=True -crashforuat -stdout -SkipSkinVerify -verbose=True -buildtexturestreaming -AllowCommandletRendering -rhithread -NOSCREENMESSAGES -run=MoveAssets -Guid=%3 -GenerateCollision=%4

    if %errorlevel% equ 0 goto :continue

    goto :while

:continue

RMDIR /s /q "%PakExportDir%\Source"
"%UtilsDir%\7-Zip\x64\7za.exe" x "%PakExportDir%\PakExportNoBuild.7z" -o%PakExportDir% -y

::cook game
call "%~dp0\CookGame.bat" "%UEDir%"
if %errorlevel% neq 0 exit /b %errorlevel%

::cook PakE help plugin
call "%~dp0\CookPakE.bat" "%UEDir%" %Guid%
if %errorlevel% neq 0 exit /b %errorlevel%

::copy pak to destination dir
copy "%SourcePakPath%" "%DestDir%"
if %errorlevel% equ 0 goto continue2

echo F|XCOPY /S /Q /Y /F "%SourcePakPath%" "%DestDir%"

:continue2

exit /b 0
