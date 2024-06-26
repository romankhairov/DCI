@echo off

set ThisDir=%~dp0

set UEDir=%2
set UEDir=%UEDir:"=%

set CompileMode=Development
IF NOT [%1] == [] set CompileMode=%1

set Param=
set Param=%Param% BuildCookRun
set Param=%Param% -project="%ThisDir%PakExport.uproject"
set Param=%Param% -noP4
set Param=%Param% -platform=Win64
set Param=%Param% -targetplatform=Win64
set Param=%Param% -clientconfig=%CompileMode%
set Param=%Param% -cook
set Param=%Param% -Compressed
set Param=%Param% -build
set Param=%Param% -noserver
set Param=%Param% -stage
set Param=%Param% -prereqs
set Param=%Param% -package
set Param=%Param% -crashreporter
set Param=%Param% -pak
set Param=%Param% -archive
set Param=%Param% -archivedirectory=%ThisDir%
set Param=%Param% -manifests
IF NOT "%3" == "Debug" set Param=%Param% -nodebuginfo
set Param=%Param% -notest
set Param=%Param% -IgnoreBuildRecords
call "%UEDir%\Engine\Build\BatchFiles\RunUAT.bat" %Param%
if %errorlevel% neq 0 exit /b %errorlevel%

IF not exist Windows goto Error

exit /b 0

:Error
exit /b 1
