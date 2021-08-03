# Golang处理JSON

JSON解析到结构体

```
package main
import (
    "encoding/json"
    "fmt"
    "os"
)
type Server struct {
    ServerName string
    ServerIP   string
}
type Serverslice struct {
    Servers []Server
}
func main() {
    var s Serverslice
    str := `{"servers":
   [{"serverName":"Guangzhou_Base","serverIP":"127.0.0.1"},
   {"serverName":"Beijing_Base","serverIP":"127.0.0.2"}]}`
    err:=json.Unmarshal([]byte(str), &s)
    if err!=nil{
        fmt.Println(err)
    }
    fmt.Println(s)
    fmt.Println(s.Servers[0].ServerName)
}
```

JSON转MAP

```
package main
import (
    "fmt"
    "encoding/json"
)
func main() {
    b := []byte(`{"IP": "127.0.0.1", "name": "SKY"}`)
    m := make(map[string]string)
    err := json.Unmarshal(b, &m)
    if err != nil {
        fmt.Println("Umarshal failed:", err)
        return
    }
    fmt.Println("m:", m)
    for k,v :=range m {
        fmt.Println(k, ":", v)
    }
}
```

生成JSON

```
package main
import (
    "encoding/json"
    "fmt"
)
type Server struct {
    ServerName string `json:"serverName,string"`
    ServerIP   string `json:"serverIP,omitempty"`
}
type Serverslice struct {
    Servers []Server `json:"servers"`
}
func main() {
    var s Serverslice
    s.Servers = append(s.Servers, Server{ServerName: "Guangzhou_Base", ServerIP: "127.0.0.1"})
    s.Servers = append(s.Servers, Server{ServerName: "Beijing_Base", ServerIP: "127.0.02"})
    b, err := json.Marshal(s)
    if err != nil {
        fmt.Println("JSON ERR:", err)
    }
    fmt.Println(string(b))
}
```
