@echo off
echo "���Ҫ�ָ���ı��ļ�������Ϊa.txt,��BAT�����TXTҪ��ͬһĿ¼��"
echo "�����밴�����зָ�TXT�ļ����س�"
set /p n=
powershell -c "$n=1;$m=1;gc 'a.txt'|%%{$f=''+$m+'.txt';$_>>$f;if($n%%%n% -eq 0){$m++};$n++}" 
pause