@echo off
echo "请把要分割的文本文件重命名为a.txt,此BAT程序和TXT要在同一目录下"
echo "请输入按多少行分割TXT文件并回车"
set /p n=
powershell -c "$n=1;$m=1;gc 'a.txt'|%%{$f=''+$m+'.txt';$_>>$f;if($n%%%n% -eq 0){$m++};$n++}" 
pause