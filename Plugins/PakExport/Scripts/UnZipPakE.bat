:: Copyright 2024 3D Source, Inc. All Rights Reserved.

@echo off

setlocal

set Guid=%1

echo Guid=%Guid%

for %%I in ("%~dp0.") do for %%J in ("%%~dpI.") do set ParentFolderName=%%~dpnxJ

set UtilsDir=%ParentFolderName%\Utils
set PakExportDir=%UtilsDir%\PakExport
set PakEDir=%PakExportDir%\Plugins\%Guid%

call %ParentFolderName%\Scripts\Cleanup.bat

cd "%UtilsDir%"

"%UtilsDir%\7-Zip\x64\7za.exe" x "%UtilsDir%\PakExport\Plugins.7z" -o%UtilsDir%\PakExport
"%UtilsDir%\7-Zip\x64\7za.exe" x "%UtilsDir%\PakExport\Content.7z" -o%UtilsDir%\PakExport

ren "%PakExportDir%\Plugins\PakE" "%Guid%"
ren "%PakEDir%\PakE.uplugin" "%Guid%.uplugin"
