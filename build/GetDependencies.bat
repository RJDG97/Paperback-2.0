@echo OFF
setlocal enabledelayedexpansion
cd %cd%
set PAPERBACK_PATH=%cd%

rem --------------------------------------------------------------------------------------------------------
rem Set the color of the terminal to blue with yellow text
rem --------------------------------------------------------------------------------------------------------
COLOR 8E
powershell write-host -fore White ------------------------------------------------------------------------------------------------------
powershell write-host -fore Cyan Welcome I am your PAPERBACK ENGINE dependency updater bot, let me get to work
powershell write-host -fore White ------------------------------------------------------------------------------------------------------
echo.



:DOWNLOAD_DEPENDENCIES
powershell write-host -fore White ------------------------------------------------------------------------------------------------------
powershell write-host -fore White PAPERBACK ENGINE - DOWNLOADING DEPENDENCIES
powershell write-host -fore White ------------------------------------------------------------------------------------------------------

echo.
rmdir "../dependencies/xcore" /S /Q
git clone https://gitlab.com/LIONant/xcore.git "../dependencies/xcore"
if %ERRORLEVEL% GEQ 1 goto :PAUSE



:COMPILATION
powershell write-host -fore White ------------------------------------------------------------------------------------------------------
powershell write-host -fore White PAPERBACK ENGINE - COMPILING DEPENDENCIES
powershell write-host -fore White ------------------------------------------------------------------------------------------------------

cd ../dependencies/xcore/builds
call UpdateDependencies.bat "return"
if %ERRORLEVEL% GEQ 1 goto :PAUSE
cd /d %PAPERBACK_PATH%

rem Uncomment the bottom part if building Tracy as well to profile the engine
rem cd ../dependencies/xcore/builds
rem call BuildTracy.bat "return"
rem if %ERRORLEVEL% GEQ 1 goto :PAUSE
rem cd /d %PAPERBACK_PATH%



:DONE
powershell write-host -fore White ------------------------------------------------------------------------------------------------------
powershell write-host -fore White PAPERBACK ENGINE - DONE!!
powershell write-host -fore White ------------------------------------------------------------------------------------------------------



:PAUSE
rem if no one give us any parameters then we will pause it at the end, else we are assuming that another batch file called us
if %1.==. pause
