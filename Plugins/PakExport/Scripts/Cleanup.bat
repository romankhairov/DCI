@echo off

setlocal

set ThisDir=%~dp0
set UtilsDir=%ThisDir%..\Utils
set PakExportDir=%UtilsDir%\PakExport

RMDIR /s /q "%PakExportDir%\Plugins"
RMDIR /s /q "%PakExportDir%\Content"

"%UtilsDir%\7-Zip\x64\7za.exe" x "%PakExportDir%\PakExport.7z" -o%PakExportDir% -y
