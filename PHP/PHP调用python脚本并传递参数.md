# 在127.0.0.1下 PHP脚本：$a  $b 即为传递的参数
```PHP
$a=1;
$b=222;
$cmd = system("python 1.py {$a} {$b}");
```

# python脚本： 求和  sys.argv[1]，sys.argv[2] 为传递的参数，必须  0指向本身
```Python
import sys
def cs(a,b):
	m=int(a)+int(b)
	return m
	pass
if __name__ == '__main__':
	res = cs(a=sys.argv[1],b=sys.argv[2])
	print (res)
```