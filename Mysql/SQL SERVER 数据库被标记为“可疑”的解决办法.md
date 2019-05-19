### 一、解决方法：当数据库发生这种操作故障时，可以按如下操作步骤可解决此方法，打开数据库里的Sql 查询编辑器窗口，运行以下的命令。
#### 1、修改数据库为紧急模式 
```Python
ALTER DATABASE [dbname] SET EMERGENCY
```
#### 2、使数据库变为单用户模式  
```Python
ALTER DATABASE [dbname] SET SINGLE_USER
```
#### 3、修复数据库日志重新生成，此命令检查的分配，结构，逻辑完整性和所有数据库中的对象错误。当您指定“REPAIR_ALLOW_DATA_LOSS”作为DBCC CHECKDB命令参数，该程序将检查和修复报告的错误。但是，这些修复可能会导致一些数据丢失 
```Python
DBCC CheckDB ([dbname], REPAIR_ALLOW_DATA_LOSS)
```
#### 4、使数据库变回为多用户模式  
```Python
ALTER DATABASE [dbname] SET MULTI_USER
```
#### ALTER DATABASE [dbname] SET SINGLE_USER这一句如果程序还在不停重连好像就会一直执行不完，我试过改成这句即可：
```Python
ALTERDATABASE [dbname] SET SINGLE_USER WITH ROLLBACK IMMEDIATE
```

### 二、MS SQL 数据库状态为SUSPECT的处理方法
```Python
当SQL SERVER数据库状态为质疑（SUSPECT）状态时，我们可以用以下方法来处理：
1. 修改数据库为紧急模式：ALTER DATABASE DBName SET EMERGENCY .
2. 检查数据库的完整性：DBCC  CHECKDB(‘DBName’)
3. 检查没有错误则恢复数据库为正常模式：ALTER DATABASE  DBName SET ONLINE;
4  如检查数据库有错误则修改数据库为单用户模式，依情况选择以下命令行进行修复数据;

DBCC CHECKDB('DBName',  REPAIR_FAST);    
DBCC CHECKDB('DBName',  REPAIR_REBUILD);    
DBCC CHECKDB('DBName',  REPAIR_ALLOW_DATA_LOSS);
```
```Python
ALTER DATABASE DBName SET EMERGENCY      /* 修改数据库为紧急模式*/
ALTER DATABASE DBName SET SINGLE_USER      /* 修改数据库为单用户模式*/
ALTER DATABASE DBName SET MULTI_USER       /* 修改数据库为多用户模式*/
ALTER DATABASE DBName SET ONLINE           /* 数据库从紧急&单用户&多用户模式恢复为正常模式*/
DBCC CHECKDB('DBName')                     /* 检查数据库完整性*/
DBCC CHECKDB('DBName', REPAIR_FAST)        /* 快速修复数据库*/
DBCC CHECKDB('DBName', REPAIR_REBUILD)     /* 重建索引并修复*/
DBCC CHECKDB('DBName', REPAIR_ALLOW_DATA_LOSS) /*如果必要允许丢失数据修复，数据库修复需在单用户模式下进行
```
