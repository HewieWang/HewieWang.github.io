## 使用 upx 压缩 go build 打包的可执行文件
go build使用的是静态编译，会将程序的依赖一起打包，这样一来编译得到的可执行文件可以直接在目标平台运行，无需运行环境（例如 JRE）或动态链接库（例如 DLL）的支持。

虽然 Go 的静态编译很方便，但也存在一个问题：打包生成的可执行文件体积较大，毕竟相关的依赖都被打包进来了。今天就来尝试一下压缩 Go 编译得到的可执行文件的体积。

### 1. 添加 -ldflags 参数
在程序编译的时候可以加上-ldflags "-s -w"参数来优化编译，原理是通过去除部分链接和调试等信息来减小编译生成的可执行程序体积，具体参数如下：

-a：强制编译所有依赖包
-s：去掉符号表信息，不过panic的时候stace trace就没有任何文件名/行号信息了
-w：去掉DWARF调试信息，不过得到的程序就不能使用gdb进行调试了
注：不建议-w和-s同时使用
```go
> go build -ldflags "-s" main.go # 添加 -s，去掉符号表
> ls -al main.exe # 约为 14.7M
-rwxr-xr-x 1 abelsu7 197609 15397888 10月 25 15:59 main.exe*

```

### 使用UPX

```go
# 设置交叉编译环境变量
> set GOOS=Linux
> set GOARCH=amd64
> set CGO_ENABLED=0

# 直接交叉编译
> go build main.go

> ls -al main # 约为 19.9M
-rw-r--r-- 1 abelsu7 197609 20837787 10月 25 16:51 main

# 开启编译优化，去掉符号表
> go build -ldflags "-s" main.go

> ls -al main # 约为 14.7M
-rw-r--r-- 1 abelsu7 197609 15464736 10月 25 16:51 main

> upx main # 压缩至原体积的 32.60%
                       Ultimate Packer for eXecutables
                          Copyright (C) 1996 - 2018
UPX 3.95w       Markus Oberhumer, Laszlo Molnar & John Reiser   Aug 26th 2018

        File size         Ratio      Format      Name
   --------------------   ------   -----------   -----------
  15464736 ->   5041696   32.60%   linux/amd64   main

Packed 1 file.

> ls -al main # 约为 4.8M
-rw-r--r-- 1 abelsu7 197609 5041696 10月 25 16:51 main

```
下面是upx的一些常用参数：

-o：指定输出的文件名
-k：保留备份原文件
-1：最快压缩，共1-9九个级别
-9：最优压缩，与上面对应
-d：解压缩decompress，恢复原体积
-l：显示压缩文件的详情，例如upx -l main.exe
-t：测试压缩文件，例如upx -t main.exe
-q：静默压缩be quiet
-v：显示压缩细节be verbose
-f：强制压缩
-V：显示版本号
-h：显示帮助信息
--brute：尝试所有可用的压缩方法，slow
--ultra-brute：比楼上更极端，very slow
