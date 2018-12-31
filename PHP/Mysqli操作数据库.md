# 数据库连接
```PHP
$db_host = 'localhost';
$db_name = 'test';
$db_user = 'root';
$db_pwd = '';
//面向对象方式
$mysqli = new mysqli($db_host, $db_user, $db_pwd, $db_name);
//面向对象的昂视屏蔽了连接产生的错误，需要通过函数来判断
if(mysqli_connect_error()){
    echo mysqli_connect_error();
}
//设置编码
$mysqli->set_charset("utf8");//或者 $mysqli->query("set names 'utf8'")
//关闭连接
$mysqli->close();

//面向过程方式的连接方式
$mysqli = mysqli_connect($db_host, $db_user, $db_pwd, $db_name);
//判断是否连接成功
if(!$mysqli ){
    echo mysqli_connect_error();
}
//关闭连接
mysqli_close($mysqli);
```
# 数据库查询
### 无结果集
```PHP 
//无结果集示例
$sql = "insert into table_name (`name`, `address`) values ('xiaoming', 'adddddrrreess')";
$result = $mysqli->query($sql);
//或者
$sql = "delete from table_name where name='xiaoming'";
$result = $mysqli->query($sql);
if($result === false){
    echo $mysqli->error;
    echo $mysqli->errno;
}
//影响条数
echo $mysqli->num_rows;
//插入的id
echo $mysqli->insert_id;
$mysqli->close();
```
### 有结果集
```PHP
$sql = "select * from table_name";
$result = $mysqli->query($sql);
if($result === false){//执行失败
    echo $mysqli->error;
    echo $mysqli->errno;
}
//行数
echo $result->num_rows;
//列数 字段数
echo $result->field_count;
//获取字段信息
$field_info_arr = $result->fetch_fields();
//移动记录指针
//$result->data_seek(1);//0 为重置指针到起始
//获取数据
while($row = $result->fetch_assoc()){
    echo $row['name'];
    echo $row['address'];
}
//也可一次性获取所有数据
//$result->data_seek(0);//如果前面有移动指针则需重置
$data = $result->fetch_all(MYSQLI_ASSOC);
$mysqli->close();
```
### 预处理示例
```PHP
$sql = "insert inro table_name ('name','address') values (?,?)";
//获得预处理对象
$stmt = $mysqli->prepare($sql);
//绑定参数 第一个参数为绑定的数据类型
/*
i:integer 整型
d:double 浮点型
s:string 字符串
b:a blob packets blob数据包
*/
$name = "xiaoming";
$address = "adddressss";
$stmt->bind_param("ss", $name, $address);//绑定时使用变量绑定
//执行预处理
$stmt->execute();
/*
//可重新绑定 多次执行
$stmt->bind_param("ss", $name, $address);
$stmt->execute();
*/
//插入的id 多次插入为最后id
echo $stmt->insert_id;
//影响行数 也是最后一次执行的
echo $stmt->affected_rows;
//错误号
echo $stmt->errno;
//错误信息
echo $stmt->error;
//关闭
$stmt->close();
$mysqli->close();
```
###下面示例select的预处理
```PHP
$sql = "select * from table_name where id<?";
$stmt = $mysqli->prepare($sql);
$id = 30;
$stmt->bind_param("i", $id);
$stmt->execute();
//获取结果集
$result = $stmt->get_result();//结果集取后的操作就和之前一样了
//获取所有数据
$data = $result->fetch_all(MYSQLI_ASSOC);
$result->close();
$mysqli->close();
```