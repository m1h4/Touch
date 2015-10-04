@ECHO OFF

ECHO Generating Font1...
..\fntgen\Release\fntgen generate C:\Windows\Fonts\zegoelight-u.ttf /o Font1.fdt /s 110 > nul
IF ERRORLEVEL 1 ECHO Error generating Font1. && PAUSE