@echo off

setlocal
set ThisDir=%~dp0
set UtilsDir=%ThisDir%..\Utils
set PakExportDir=%UtilsDir%\PakExport
set PakExportProjectFile=%PakExportDir%\PakExport.uproject
set PakESavedDir=%PakExportDir%\Plugins\PakE\Saved
set SourcePakPath=%PakESavedDir%\StagedBuilds\Windows\PakExport\Plugins\PakE\Content\Paks\Windows\PakEPakExport-Windows.pak

set UEDir=%1
set UEDir=%UEDir:"=%
set DestDir=%2
set DestDir=%DestDir:"=%

echo UEDir=%UEDir%
echo DestDir=%DestDir%

::fix root path
"%UtilsDir%\fnr.exe" --cl --dir "%PakExportDir%\Plugins\PakE\Content" --fileMask "*.*" --includeSubDirectories --caseSensitive --KeepModifiedDate --skipBinaryFileDetection --alwaysUseEncoding "windows-1251" --find "Game" --replace "PakE"

::cook game
call "%~dp0\CookGame.bat" "%UEDir%"

::cook PakE help plugin
call "%~dp0\CookPakE.bat" "%UEDir%"

::copy pak to destination dir
echo F|XCOPY /S /Q /Y /F "%SourcePakPath%" "%DestDir%"

exit /b
