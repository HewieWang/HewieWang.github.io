@echo off
title ��ӷ���ǽ����˿�
echo           -----------------------------------------------------------
echo          ^|                                                           ^|
echo          ^|                                                           ^|
echo          ^|              ��ʾ:�˿����ֵ������65535                   ^|
echo          ^|        Ҫ����һ�������˿ڣ�������50001-50009��ʽ          ^|
echo          ^|             ����������˿ڣ���ֱ�������                ^|
echo           -----------------------------------------------------------
echo.
echo off
set /p port=������Ҫ����Ķ˿�:
netsh advfirewall firewall add rule name="allow%port%" protocol=TCP dir=in localport=%port% action=allow
netsh advfirewall firewall add rule name="allow%port%" protocol=TCP dir=out localport=%port% action=allow
pause