```Bash
from pyquery import PyQuery as pq
doc = pq(url='http://cuiqingcai.com')
print(doc('title'))
```
```Bash
from pyquery import PyQuery as pq
doc = pq(filename='demo.html')
print(doc('li'))
```