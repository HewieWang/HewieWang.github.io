# JS过滤特殊字符
```Javascript
function checkinput(str){
	newstr=str.replace(/\s+/g,"");
	if(newstr==''){alert('请输入!');return false;}
	else if(newstr.indexOf("'")!=-1||newstr.indexOf("\"")!=-1||newstr.indexOf("@")!=-1)
	{alert('请勿输入特殊字符!');return false;}
	else{return newstr;}
}
```

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

# 数组求和
```Javascript
let arr = [1, 2, 3, 4, 5]
function sum(arr) {
return eval(arr.join("+"))
}
sum(arr)
```

# 取出一个数组中的最大值和最小值
```Javascript
var numbers = [5, 458 , 120 , -215 , 228 , 400 , 122205, -85411]; 
var maxInNumbers = Math.max.apply(Math, numbers); 
var minInNumbers = Math.min.apply(Math, numbers);
```

# 获取链接 https://www.baidu.com?name=jawil&age=23 name的value值
```Javascript
function getParamName(attr) {
  let match = RegExp(`[?&]${attr}=([^&]*)`) //分组运算符是为了把结果存到exec函数返回的结果里
    .exec(window.location.search)
  //["?name=jawil", "jawil", index: 0, input: "?name=jawil&age=23"]
  return match && decodeURIComponent(match[1].replace(/\+/g, ' ')) // url中+号表示空格,要替换掉
}
console.log(getParamName('name'))
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

# 去掉字符串左右两边的空格，" jaw il " --> “jaw il”
```Javascript
function trim(str) {
    return str.replace(/(^\s*)|(\s*$)/g, "")
}
let str = "  jaw il "
console.log(trim(str))
```

# 字符串数组去重 ["a","b","c","a","b","c"] --> ["a","b","c"]
```Javascript
var str_arr = ["a", "b", "c", "a", "b", "c"]
function unique(arr) {
    return arr.sort().join(",,").
    replace(/(,|^)([^,]+)(,,\2)+(,|$)/g, "$1$2$4").
    replace(/,,+/g, ",").
    replace(/,$/, "").
    split(",")
}
console.log(unique(str_arr))
```

# JS判断字符串是否包含某个字符串
```Javascript
var str = "123";
console.log(str.indexOf("3")!=-1);
```
