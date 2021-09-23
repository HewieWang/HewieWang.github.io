### golang 逐行读取文件
```Go
package main

import (
    "bufio"
    "fmt"
    "io"
    "os"
)

func main() {
    fi, err := os.Open("C:/Documents and Settings/xxx/Desktop/tax.txt")
    if err != nil {
        fmt.Printf("Error: %s\n", err)
        return
    }
    defer fi.Close()

    br := bufio.NewReader(fi)
    for {
        a, _, c := br.ReadLine()
        if c == io.EOF {
            break
        }
        fmt.Println(string(a))
    }
}
```
### Go语言截取指定字符串中间字符串的方法
```Go
import "strings"
 
func GetBetweenStr(str, start, end string) string {
 n := strings.Index(str, start)
 if n == -1 {
 n = 0
 }
 str = string([]byte(str)[n:])
 m := strings.Index(str, end)
 if m == -1 {
 m = len(str)
 }
 str = string([]byte(str)[:m])
 return str
}
```
### 将string转换为io.Reader类型
```Go
bytes.NewReader([]byte("aaaa"))
```
### golang 获取所有Hhttps://开头的URL
```Go
package main

import (
    "fmt"
    "regexp"
    "os"
    "io/ioutil"
    "strings"
)

func main() {
	file, err := os.Open(os.Args[1])
	if err != nil {
		panic(err)
	}
	defer file.Close()
	content, err := ioutil.ReadAll(file)
	buf := string(content)
	reg1 := regexp.MustCompile(`https://[^\s]*`)
	if reg1 == nil {
		fmt.Println("regexp err")
		return
	}
	result1 := reg1.FindAll([]byte(buf), -1)
	for _, m := range result1 {
		x:=strings.Replace(string(m)[:strings.Index(string(m),"\"")], "\\", "", -1)
		fmt.Println(string(x))
	}
}
```
