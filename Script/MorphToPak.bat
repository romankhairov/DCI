@echo off

setlocal
set ThisDir=%~dp0
set "ThisDir=%ThisDir:\=/%"
set UtilsDir=%ThisDir%../Utils

set UEDir=%1
set UEDir=%UEDir:"=%
set Destination=%2
set Destination=%Destination:"=%
set UECmdDir=%UEDir%/Binaries/Win64/UE4Editor-Cmd.exe
set DSImportDir=%UtilsDir%/DSImport
set AssetDir=%DSImportDir%/Content/Meshes
set PakExportDir=%ThisDir%../Plugins/PakExport
set PakExportContentDir=%PakExportDir%/Utils/PakExport/Plugins/PakE/Content
set PakExportScriptsDir=%PakExportDir%/Scripts

::remove template project
RMDIR /s /q "%DSImportDir%"

::unzip template project
call "%ThisDir%/UnZipDSImport.bat"

::perform morph to static
"%UECmdDir%" "%DSImportDir%/DSImport.uproject" -IgnoreChangeList map=/Game/BaseMap.umap -UNATTENDED -RUNNINGUNATTENDEDSCRIPT -collision=True -Save -rebuild -nosourcecontrol -previewsurface=True -crashforuat -stdout -SkipSkinVerify -verbose=True -buildtexturestreaming -AllowCommandletRendering -rhithread -NOSCREENMESSAGES -run=MorphToPak -Length=%3 -Width=%4 -Edge=%5 -SizeX=%6 -SizeY=%7 -Gap=%8 -Styling=%9

::perform cut plank
"%UECmdDir%" "%DSImportDir%/DSImport.uproject" -IgnoreChangeList map=/Game/BaseMap.umap -UNATTENDED -RUNNINGUNATTENDEDSCRIPT -collision=True -Save -rebuild -nosourcecontrol -previewsurface=True -crashforuat -stdout -SkipSkinVerify -verbose=True -buildtexturestreaming -AllowCommandletRendering -rhithread -NOSCREENMESSAGES -run=pythonscript -script="%ThisDir%/MorphToPak.py %3 %4 %5 %6 %7 %8 %9

::unzip PakExportPlugin template project
call "%PakExportScriptsDir%/UnZip.bat"

::copy asset to PakExportPlugin
echo F|XCOPY /S /Q /Y /F "%AssetDir%" "%PakExportContentDir%"

::remove template project
RMDIR /s /q "%DSImportDir%"

::create asset pak file
call "%PakExportScriptsDir%/CookAndPak.bat" "%UEDir%" "%Destination%.pak"

::cleamup
call "%PakExportScriptsDir%/Cleanup.bat"
