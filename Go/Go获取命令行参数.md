### Low B 方式
```go
package main
import (
    "fmt"
    "os"
)
func main() {
    fmt.Println("命令行的参数有", len(os.Args))
    // 遍历 os.Args 切片，就可以得到所有的命令行输入参数值
    for i, v := range os.Args {
        fmt.Printf("args[%v]=%v\n", i, v)
    }
}
```
![](https://img2018.cnblogs.com/blog/1468833/201905/1468833-20190530223228857-92788537.png)

### 高级方式　 flag包实现了命令行参数的解析：

```go
package main
import (
    "flag"
    "fmt"
)
func main() {
    // 定义几个变量，用于接收命令行的参数值
    var user        string
    var password    string
    var host        string
    var port        int
    // &user 就是接收命令行中输入 -u 后面的参数值，其他同理
    flag.StringVar(&user, "u", "root", "账号，默认为root")
    flag.StringVar(&password, "p", "", "密码，默认为空")
    flag.StringVar(&host, "h", "localhost", "主机名，默认为localhost")
    flag.IntVar(&port, "P", 3306, "端口号，默认为3306")
    // 解析命令行参数写入注册的flag里
    flag.Parse()
    // 输出结果
    fmt.Printf("user：%v\npassword：%v\nhost：%v\nport：%v\n",
        user, password, host, port)
}
```
![](https://img2018.cnblogs.com/blog/1468833/201905/1468833-20190530224558066-1728416704.png)
