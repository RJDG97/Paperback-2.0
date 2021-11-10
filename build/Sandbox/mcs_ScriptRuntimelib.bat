@echo off
"%~dp0\Dependencies\mono\bin\mono.exe" %MONO_OPTIONS% "%~dp0\Dependencies\mono\lib\mono\4.5\mcs.exe" %* -t:library -out:Dependencies\ScriptRuntimelib.dll ..\CSScript\*.cs ..\CSScript\Tools\*.cs > mcs_ScriptRuntimelib_output.txt 2>&1
