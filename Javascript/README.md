# 优雅的取随机字符串
```Javascript
Math.random().toString(16).substring(2) 
Math.random().toString(36).substring(2)
```

# 最短的代码实现数组去重
```Javascript
[...new Set([1, "1", 2, 1, 1, 3])]
```

# 用最短的代码实现一个长度为m(6)且值都n(8)的数组
```Javascript
Array(6).fill(8)
```

# 取出一个数组中的最大值和最小值
```Javascript
var numbers = [5, 458 , 120 , -215 , 228 , 400 , 122205, -85411]; 
var maxInNumbers = Math.max.apply(Math, numbers); 
var minInNumbers = Math.min.apply(Math, numbers);
```

# 从一个数组中找到一个数，O(n)的算法，找不到就返回 null
```Javascript
const find = ( f => f(f) ) ( f =>
  (next => (x, y, i = 0) =>
    ( i >= x.length) ?  null :
      ( x[i] == y ) ? i :
        next(x, y, i+1))((...args) =>
          (f(f))(...args)))
 
let arr = [0,1,2,3,4,5]
console.log(find(arr, 2))
console.log(find(arr, 8))
```

# 让两个整数交换数值
```Javascript
a ^= b;
b ^= a;
a ^= b;
```
```Javascript
var a = 3,b = 4;
[b, a] = [a, b]
```

# 取整
```Javascript
var a = ~~2.33
var b= 2.33 | 0
var c= 2.33 >> 0
```
