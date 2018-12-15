@echo off
title 添加防火墙允许端口
echo           -----------------------------------------------------------
echo          ^|                                                           ^|
echo          ^|                                                           ^|
echo          ^|              提示:端口最大值不超过65535                   ^|
echo          ^|        要允许一段连续端口，请输入50001-50009格式          ^|
echo          ^|             多个不连续端口，请分别多次输入                ^|
echo           -----------------------------------------------------------
echo.
echo off
set /p port=请输入要允许的端口:
netsh advfirewall firewall add rule name="allow%port%" protocol=TCP dir=in localport=%port% action=allow
netsh advfirewall firewall add rule name="allow%port%" protocol=TCP dir=out localport=%port% action=allow
pause