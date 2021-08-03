# Golang——for循环的两种用法

Go也支持传统的写法`for i := 0; i < count; i++ {...}`。同样，还有一种`for i, a := range aa {...}`。
这个跟Java相比，除了能够遍历，还能得到当前遍历的索引值，所以在Go中，更倾向于后面这种方法，灵活度更高。

今天要对一个数据进行遍历，对每个元素进行稍加修改，才发现，原来这两种方式是有很大区别的。当时使用第二种方式修改之后，一直发现没有修改成功。后来进过调试，才发现是`for`循环的问题。

测试代码在

```Go
a := []string{}
a = append(a, "hello")
a = append(a, ", ")
a = append(a, "world")
fmt.Println(a)

for i := 0; i < len(a); i++ {
fmt.Println(&a[i])
}

for _, aa := range a {
aa += "@"
fmt.Println(&aa)
}
for i := 0; i < len(a); i++ {
a[i] += "@"
fmt.Println(&a[i])
}
fmt.Println(a)
```
