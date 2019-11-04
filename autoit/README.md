### 下载地址
```Bash
https://www.autoitscript.com/site/autoit/downloads/
```
### 点击EXE里面按钮
```Bash
ControlClick("腾讯企业邮箱小助手", "一键登录","TButton4");       //title   text class
```
### 运行Dos命令
```Bash
#include <Process.au3>
$runAdb = _RunDOS("adb shell logcat >e:\logcat.txt")
```
### 接收外部命令行参数
```Bash
$CmdLine[0] ;参数的数量
$CmdLine[1] ;第一个参数 (脚本名称后面)
$CmdLine[2] ;第二个参数等等
$CmdLine[$CmdLine[0]] ;可以用来表示得到最后一个参数
```
### 循环
```Bash
Dim $i
For $i = 0 To 5 Step 1
    MsgBox ( 0 , " 变量 $i 的值 " , " 变量 $i = " & $i)
Next
MsgBox ( 0 , "$i 最后的值 " , "$i = " & $i)
```