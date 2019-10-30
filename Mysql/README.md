# 清空表资料
```SQL
TRUNCATE TABLE "表格名";
```
# 添加用户
```SQL
CREATE USER 'username'@'host' IDENTIFIED BY 'password';   //CREATE USER 'dog'@'localhost' IDENTIFIED BY '123456';
```
# 授权
```SQL
GRANT privileges ON databasename.tablename TO 'username'@'host'
//说明: privileges – 用户的操作权限,如SELECT , INSERT , UPDATE 等(详细列表见该文最后面).如果要授予所 的权限则使用ALL.;databasename – 数据库名,tablename-表名,如果要授予该用户对所有数据库和表的相应操作权限则可用* 表示
GRANT SELECT, INSERT ON test.user TO 'pig'@'%';
GRANT ALL ON *.* TO 'pig'@'%';
```
# 设置与更改用户密码
```SQL
SET PASSWORD FOR 'username'@'host' = PASSWORD('newpassword');
//如果是当前登陆用户用
SET PASSWORD = PASSWORD("newpassword");
```
# 删除用户
```SQL
DROP USER ‘username’@'host’;
```