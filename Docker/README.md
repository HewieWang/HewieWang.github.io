### Docker-MySql — 使用mysqldump 命令备份导出mysql容器中的结构数据
1. 查看当前启动的mysql运行容器

docker ps   
2. 使用以下命令备份导出数据库中的所有表结构和数据

docker exec -it  mysql mysqldump -uroot -p123456 paas_portal > /cloud/sql/paas_portal.sql  
3.只导数据不导结构

mysqldump　-t　数据库名　-uroot　-p　>　xxx.sql　
docker exec -it mysql mysqldump -t -uroot -p123456 paas_portal >/cloud/sql/paas_portal_dml.sql  
4. 只导结构不导数据

mysqldump　--opt　-d　数据库名　-u　root　-p　>　xxx.sql　
docker exec -it mysql mysqldump  --opt -d   -uroot -p123456 paas_portal >/cloud/sql/paas_portal_ddl.sql   
5. 导出特定表的结构

mysqldump　-uroot　-p　-B　数据库名　--table　表名　>　xxx.sql
docker exec -it mysql mysqldump -uroot -p -B paas_portal --table user > user.sql 