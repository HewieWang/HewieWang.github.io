### 查看当前 Go 版本支持的编译平台

```
go tool dist list
```

```
go env
```

最常用的大概是`x86`和`amd64`架构：

* `darwin/386`：对应 **Mac x86**
* `darwin/amd64`：对应 **Mac amd64**
* `linux/386`：对应 **Linux x86**
* `linux/amd64`：对应 **Linux amd64**
* `Windows/386`：对应 **Windows x86**
* `Windows/amd64`：对应 **Windows amd64**

在 **Windows** 下编译 **MacOS** 和 **Linux** 的 **64 位**程序：

```
# For MacOS/amd64
set CGO_ENABLED=0
set GOOS=darwin
set GOARCH=amd64
go build main.go

# For Linux/amd64
set CGO_ENABLED=0
set GOOS=linux
set GOARCH=amd64
go build main.go
```

在 **Linux** 下编译 **MacOS** 和 **Windows** 的 **64 位**程序：

```
# For MacOS/amd64
CGO_ENABLED=0 GOOS=darwin GOARCH=amd64 go build main.go

# For Windows/amd64
CGO_ENABLED=0 GOOS=windows GOARCH=amd64 go build main.go
```

在 **MacOS** 下编译 **Windows** 和 **Linux** 的 **64 位**程序：

```
# For Windows/amd64
CGO_ENABLED=0 GOOS=windows GOARCH=amd64 go build main.go

# For Linux/amd64
CGO_ENABLED=0 GOOS=linux GOARCH=amd64 go build main.go
```

### 选择性编译

虽然 Golang 可以跨平台编译，但**系统之间的差异性仍然存在**。有些时候我们会直接调用操作系统函数，**不同操作系统下的库可能会有不同的实现**，比如`syscall`库。

命令`go build`没有内置`#define`或者预处理器之类的处理平台相关的代码取舍，而是采用 **Tag 标记**和**文件后缀**的方式实现**选择性编译**。

#### 3.1 构建标记：build tag

为了实现**根据不同的目标平台编译对应的源文件**，需要在**文件顶部**添加**构建标记**`build tag`：

```
// +build
```

标记遵循以下规则：

1. A build tag is evaluated as the **OR** of **space**-separated options
2. Each option evaluates as the **AND** of its **comma**-separated terms
3. Each term is an alphanumeric word or, preceded by **!**, its **negation**

简单翻译一下：

1. **空格**`<span> </span>`为**或**
2. **逗号**`,`为**且**
3. **叹号**`!`为**非**

例如：

```
// +build !windows,386
//此文件在非 Windows 操作系统，且为 x86 处理器时编译
```

```
// +build linux darwin
// +build 386
```

另外需要注意`build tag`和`package xxx`语句之间需要有**空行分隔**，也就是：

```
// +build linux darwin
// +build 386

package mypkg
```

#### 文件后缀：_$GOOS.go

```
以`_$GOOS.go`为后缀的文件**只在此平台上编译**，其他平台上编译时就**当此文件不存在**
```

例如：

* `syscall_linux_amd64.go`：只在 **Linux/amd64** 下编译
* `syscall_windows_386.go`：只在 **Windows/i386** 下编译
* `syscall_windows.go`：只在 **Windows** 下编译
