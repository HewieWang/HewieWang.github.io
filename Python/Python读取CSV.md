## Python读取CSV

直接用open()打开

```
with open("./data.csv") as f:
    for line in f:
        print(line)
```

用python自带的标准库读取

```
import csv
csv_reader = csv.reader(open("./data.csv"))
for line in csv_reader:
    print(line)
```

用pandas读取

```
# 返回的是一个DataFrame数据
pd_reader = pd.read_csv("./data.csv")
print(pd_reader)
```

另外一种比较通用的读取数据表的方法（指定分隔符）

```
#sep 指定分隔符
data = pd.read_table("./data.csv",sep=",")
print(data)
```

summary
当我们取得dataframe对象后，就可以对二维表进行一系列操作然后再写回csv文件，实现对数据的加工和处理。
当然pandas不只局限在对csv文件的操作，同样可以对excel，json，xml格式的数据。
