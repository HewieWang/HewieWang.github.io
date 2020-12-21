mysql 查询重复字段,及删除重复记录的方法
MySQL, 数据库, 数据库, 字段, 服务器
数据库中有个大表，需要查找其中的名字有重复的记录id，以便比较。如果仅仅是查找数据库中name不重复的字段，很容易:
SELECT min(`id`),`name` FROM `table` GROUP BY `name`;

但是这样并不能得到说有重复字段的id值。（只得到了最小的一个id值）查询哪些字段是重复的也容易:
SELECT `name`,count(`name`) as count FROM `table` GROUP BY `name` HAVING count(`name`) >1 ORDER BY count DESC;

但是要一次查询到重复字段的id值，就必须使用子查询了，于是使用下面的语句。
```Python
SELECT `id`,`name` FROM `table` WHERE `name` in (
SELECT `name`
FROM `table`
GROUP BY `name` HAVING count(`name`) >1);
```

但是这条语句在mysql中效率太差，感觉mysql并没有为子查询生成零时表。于是使用先建立零时表:
```Python
create table `tmptable` as (
SELECT `name`
FROM `table`
GROUP BY `name` HAVING count(`name`) >1);
```

然后使用多表连接查询：
```Python
SELECT a.`id`, a.`name` FROM `table` a, `tmptable` t WHERE a.`name` = t.`name`;
```

结果这次结果很快就出来了。
＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝＝
查询及删除重复记录的方法
(一)
1、查找表中多余的重复记录，重复记录是根据单个字段（peopleId）来判断
```Python
select * from people
where peopleId in (select peopleId from people group by peopleId having count(peopleId) > 1)
```

2、删除表中多余的重复记录，重复记录是根据单个字段（peopleId）来判断，只留有rowid最小的记录
delete from people
where peopleId in (select peopleId from people group by peopleId having count(peopleId) > 1)
and rowid not in (select min(rowid) from people group by peopleId having count(peopleId )>1)

(上面这条语句在mysql中执行会报错：

执行报错：1093 - You can't specify target table 'student' for update in FROM clause
原因是：更新数据时使用了查询，而查询的数据又做了更新的条件，mysql不支持这种方式。oracel和msserver都支持这种方式。
怎么规避这个问题？
再加一层封装,
```Python
delete from php_user where
username in (select username from ( select username from php_user group by username having count(username)>1) a)
and id not in ( select min(id) from (select min(id) as id from php_user group by username having count(username)>1 ) b)
```

注意select min(id) 后面要有as id.

3、查找表中多余的重复记录（多个字段）
```Python
select * from vitae a
where (a.peopleId,a.seq) in (select peopleId,seq from vitae group by peopleId,seq having count(*) > 1)
```

4、删除表中多余的重复记录（多个字段），只留有rowid最小的记录
```Python
delete from vitae a
where (a.peopleId,a.seq) in (select peopleId,seq from vitae group by peopleId,seq having count(*) > 1)
and rowid not in (select min(rowid) from vitae group by peopleId,seq having count(*)>1)
```

5、查找表中多余的重复记录（多个字段），不包含rowid最小的记录
```Python
select * from vitae a
where (a.peopleId,a.seq) in (select peopleId,seq from vitae group by peopleId,seq having count(*) > 1)
and rowid not in (select min(rowid) from vitae group by peopleId,seq having count(*)>1)
```

(二)
比方说
在A表中存在一个字段「name」，
而且不同记录之间的「name」值有可能会相同，
现在就是需要查询出在该表中的各记录之间，「name」值存在重复的项；
Select Name,Count(*) From A Group By Name Having Count(*) > 1

如果还查性别也相同大则如下:
Select Name,sex,Count(*) From A Group By Name,sex Having Count(*) > 1

(三)
方法一
```Python
declare @max integer,@id integer
declare cur_rows cursor local for select 主字段,count(*) from 表名 group by 主字段 having count(*) >； 1
open cur_rows
fetch cur_rows into @id,@max
while @@fetch_status=0
begin
select @max = @max -1
set rowcount @max
delete from 表名 where 主字段 = @id
fetch cur_rows into @id,@max
end
close cur_rows
set rowcount 0
```


方法二

有两个意义上的重复记录，一是完全重复的记录，也即所有字段均重复的记录，二是部分关键词段重复的记录，比如Name字段重复，而其它字段不一定重复或都重复可以忽略。

1、对于第一种重复，比较容易解决，使用

select distinct * from tableName

就可以得到无重复记录的结果集。

如果该表需要删除重复的记录（重复记录保留1条），可以按以下方法删除
```Python
select distinct * into #Tmp from tableName
drop table tableName
select * into tableName from #Tmp
drop table #Tmp
```


发生这种重复的原因是表设计不周产生的，增加唯一索引列即可解决。

2、这类重复问题通常要求保留重复记录中的第一条记录，操作方法如下

假设有重复的字段为Name,Address，要求得到这两个字段唯一的结果集
```Python
select identity(int,1,1) as autoID, * into #Tmp from tableName
select min(autoID) as autoID into #Tmp2 from #Tmp group by Name,autoID
select * from #Tmp where autoID in(select autoID from #tmp2)
```

最后一个select即得到了Name，Address不重复的结果集（但多了一个autoID字段，实际写时可以写在select子句中省去此列）

(四)
查询重复
```Python
select * from tablename where id in (
select id from tablename
group by id
having count(id) > 1
)
```