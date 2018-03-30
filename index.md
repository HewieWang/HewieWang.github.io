### AJAX
	$.ajax({
			url: "/path/to/file",//请求地址
			type: "POST",//请求方式
			dataType: "JSON",//返回数据类型
			data: {param1: "value1"},//发送的参数
			success:function(data){
				//成功执行的方法
				alert("success");
			},
			error:function(){
				//失败执行的方法
				alert("error");
			}
		})

### Mysqli
     <?php
     $mysqli = new mysqli('localhost', 'my_user', 'my_password', 'world');
    /* check connection */
    if (mysqli_connect_errno()) {
        printf("Connect failed: %s\n", mysqli_connect_error());
        exit();
    }

    $stmt = $mysqli->prepare("INSERT INTO CountryLanguage VALUES (?, ?, ?, ?)");
    //?占位符
    $stmt->bind_param('sssd', $code, $language, $official, $percent);
    //sssd,前三个是字符串，最后一个是小数
    $code = 'DEU';
    $language = 'Bavarian';
    $official = "F";
    $percent = 11.2;

    /* execute prepared statement */
    $stmt->execute();

    printf("%d Row inserted.\n", $stmt->affected_rows);

    /* close statement and connection */
    $stmt->close();

    /* Clean up table CountryLanguage */
    $mysqli->query("DELETE FROM CountryLanguage WHERE Language='Bavarian'");
    printf("%d Row deleted.\n", $mysqli->affected_rows);

    /* close connection */
    $mysqli->close();
    ?>
    ###Mysql
    连接数据库
      mysql_connect(server,user,pwd,newlink,clientflag)
      如果成功，则返回一个 MySQL 连接标识，失败则返回 FALSE。

      选择数据库
      mysql_select_db(database,connection)
      如果成功，则该函数返回 true。如果失败，则返回 false。

      错误代码
      mysql_errno(connection)
      返回上一个MySQL函数的错误号码，如果没有出错则返回 0（零）。

      错误文本
      mysql_error(connection)
      本函数返回上一个MySQL函数的错误文本，如果没有出错则返回 ""（空字符串）。

      发送查询
      mysql_query(query,connection)
      mysql_query() 仅对 SELECT，SHOW，EXPLAIN 或DESCRIBE 语句返回一个资源标识符，如果查询执行不正确则返回 FALSE。对于其它类型的 SQL 语句，mysql_query() 在执行成功时返回 TRUE，出错时返回 FALSE。

      通过结果集取数据
      mysql_fetch_row(data)
      函数从结果集中取得一行作为数字数组。
      mysql_fetch_assoc(data)
      函数从结果集中取得一行作为关联数组。
      mysql_fetch_array(data,array_type)
      函数从结果集中取得一行作为关联数组，或数字数组，或二者兼有参数（MYSQL_ASSOC - 关联数组,MYSQL_NUM - 数字数组,MYSQL_BOTH - 默认。同时产生关联和数字数组）

      释放结果内存
      mysql_free_result(data) 
       函数释放结果内存。

      关闭链接
      mysql_close(link_identifier) 
       函数关闭非持久的 MySQL 连接。

      结果集数量
      mysql_num_rows(data)
      函数返回结果集中行的数目(select)。

      影响记录数量
      mysql_affected_rows(link_identifier) 
       函数返回前一次 MySQL 操作所影响的记录行数(UPDATE,DELETE,INSERT)

      自增ID
      mysql_insert_id(connection) 
       函数返回上一步 INSERT 操作产生的 ID

### 文件上传
	    <?php
	      if ((($_FILES["file"]["type"] == "image/gif")
	      || ($_FILES["file"]["type"] == "image/jpeg")
	      || ($_FILES["file"]["type"] == "image/pjpeg"))
	      && ($_FILES["file"]["size"] < 20000)){
		  if ($_FILES["file"]["error"] > 0){
		    echo "Return Code: " . $_FILES["file"]["error"] . "<br />";
		  }else{
		    echo "Upload: " . $_FILES["file"]["name"] . "<br />";
		    echo "Type: " . $_FILES["file"]["type"] . "<br />";
		    echo "Size: " . ($_FILES["file"]["size"] / 1024) . " Kb<br />";
		    echo "Temp file: " . $_FILES["file"]["tmp_name"] . "<br />";

		    if (file_exists("upload/" . $_FILES["file"]["name"])){
			echo $_FILES["file"]["name"] . " already exists. ";
		    }else{
			move_uploaded_file($_FILES["file"]["tmp_name"],
			"upload/" . $_FILES["file"]["name"]);
			echo "Stored in: " . "upload/" . $_FILES["file"]["name"];
		    }
		  }
		}else{
		  echo "Invalid file";
		}
	    ?>
### 文件下载
	  <?php 
	      $file="./Upload/575e163e8938f.pptx";
	      header("Content-type:application/octet-stream");//二进制文件
	      $fileName = basename($file);//获得文件名
	      header("Content-Disposition:attachment;filename=<?php echo ($fileName); ?>");//下载窗口中显示的文件名
	      header("Accept-ranges:bytes");//文件尺寸单位  
	      header("Accept-length:".filesize($file));//文件大小  
	      readfile($file);//读出文件内容
	    ?>
### 正则
    <?php
      $preg= "/xuebingsi/";
      $c = preg_match($preg,"学并思网址xuebingsi.com");
      echo $c;
      ?>

