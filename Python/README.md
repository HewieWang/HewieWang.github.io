# 有用的Python代码

### 生成8位随机字符串

str = ''.join(random.sample(string.ascii_letters + string.digits, 8))

### 生成9位随机数

num = ''.join(str(i) for i in random.sample(range(0,9),9))