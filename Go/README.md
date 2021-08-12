### golang 逐行读取文件
```Javascript
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
### golang 获取所有Hhttps://开头的URL
```Javascript
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
