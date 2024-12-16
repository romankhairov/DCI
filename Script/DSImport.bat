@echo off

setlocal
set ThisDir=%~dp0
set "ThisDir=%ThisDir:\=/%"
set UtilsDir=%ThisDir%../Utils

set UEDir=%1
set UEDir=%UEDir:"=%
set DSFile=%2
set DSFile=%DSFile:"=%
set UECmdDir=%UEDir%/Binaries/Win64/UE4Editor-Cmd.exe
set DSImportDir=%UtilsDir%/DSImport
set AssetDir=%DSImportDir%/Content/ImportedDS
set PakExportDir=%ThisDir%../Plugins/PakExport
set PakExportContentDir=%PakExportDir%/Utils/PakExport/Plugins/PakE/Content
set PakExportScriptsDir=%PakExportDir%/Scripts

::remove template project
RMDIR /s /q "%DSImportDir%"

::unzip template project
call "%ThisDir%/UnZipDSImport.bat"

::perform import datasmith
"%UECmdDir%" "%DSImportDir%/DSImport.uproject" -run=pythonscript -script="%ThisDir%/DSImport.py %DSFile%"

::unzip PakExportPlugin template project
call "%PakExportScriptsDir%/UnZip.bat"

::copy asset to PakExportPlugin
echo F|XCOPY /S /Q /Y /F "%AssetDir%" "%PakExportContentDir%"

::remove template project
RMDIR /s /q "%DSImportDir%"

::create asset pak file
call "%PakExportScriptsDir%/CookAndPak.bat" "%UEDir%" "%DSFile%.pak"

::cleamup
call "%PakExportScriptsDir%/Cleanup.bat"
