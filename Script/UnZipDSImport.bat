@echo off

setlocal
set ThisDir=%~dp0
set UtilsDir=%ThisDir%..\Utils
set DSImportDir=%UtilsDir%\DSImport
RMDIR /s /q "%DSImportDir%"
cd "%ThisDir%"
Call :UnZipFile "%UtilsDir%\" "%UtilsDir%\DSImport.zip"
exit /b

:UnZipFile <ExtractTo> <newzipfile>
set vbs="%temp%\_.vbs"
if exist %vbs% del /f /q %vbs%
>%vbs%  echo Set fso = CreateObject("Scripting.FileSystemObject")
>>%vbs% echo destPath = fso.GetAbsolutePathName(%1)
>>%vbs% echo strZipPath = fso.GetAbsolutePathName(%2)
>>%vbs% echo If NOT fso.FolderExists(destPath) Then
>>%vbs% echo fso.CreateFolder(destPath)
>>%vbs% echo End If
>>%vbs% echo set objShell = CreateObject("Shell.Application")
>>%vbs% echo set FilesInZip=objShell.NameSpace(strZipPath).items
>>%vbs% echo objShell.NameSpace(destPath).CopyHere(FilesInZip)
>>%vbs% echo Set fso = Nothing
>>%vbs% echo Set objShell = Nothing
cscript //nologo %vbs%
if exist %vbs% del /f /q %vbs%
