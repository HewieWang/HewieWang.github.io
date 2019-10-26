### (宽度自适应)：在网页代码的头部，加入一行viewport元标签
```Bash
<meta name="viewport" content="width=device-width,initial-scale=1" />
```
### (字体自适应)：字体也不能使用绝对大小（px），而只能使用相对大小（em）
```Bash
body {font: normal 100% Helvetica, Arial,sans-serif;}
```
### (流动布局)：各个区块的位置都是浮动的，不是固定不变的
```Bash
.main {float: right;width: 70%; }.leftBar {float: left;width: 25%;}
```
### (自适应网页设计)：的核心，就是CSS3引入的MediaQuery模块。它的意思就是，自动探测屏幕宽度，然后加载相应的CSS文件
```Bash
<link rel="stylesheet" type="text/css"media="screen and (max-device-width:400px)"href="tinyScreen.css" />
<link rel="stylesheet" type="text/css"media="screen and (min-width: 400px)and (max-device-width: 600px)"href="smallScreen.css" />
```
### (@media规则 )：同一个CSS文件中，也可以根据不同的屏幕分辨率，选择应用不同的CSS规则
```Bash
@media screen and (max- width: 400px) {}
```
### (图片自适应 )：在html里面插入图片，如果想让图片自适应屏幕的小而不是宽高固定不变可以在css代码里加入
```Bash
img{height: auto; width: auto\9; width:100%;}
```