### 可以替代selenium的Pyppeteer,免去配置环境,驱动
```Bash
import asyncio
from pyppeteer import launch
from pyquery import PyQuery as pq
 
async def main():
    browser = await launch()
    page = await browser.newPage()
    await page.goto('http://quotes.toscrape.com/js/')
    doc = pq(await page.content())
    print('Quotes:', doc('.quote').length)
    await browser.close()
 
asyncio.get_event_loop().run_until_complete(main())
```
#### 运行结果：
```Bash
Quotes: 10
```
```Bash
Pyppeteer 整个流程就完成了浏览器的开启、新建页面、页面加载等操作,在这个过程中，我们没有配置 Chrome 浏览器，没有配置浏览器驱动，免去了一些繁琐的步骤，同样达到了 Selenium 的效果，还实现了异步抓取
```
##### 模拟网页截图，保存 PDF，另外还可以执行自定义的 JavaScript 获得特定的内容，代码如下：
```Bash
import asyncio
from pyppeteer import launch
 
async def main():
    browser = await launch()
    page = await browser.newPage()
    await page.goto('http://quotes.toscrape.com/js/')
    await page.screenshot(path='example.png')
    await page.pdf(path='example.pdf')
    dimensions = await page.evaluate('''() => {
        return {
            width: document.documentElement.clientWidth,
            height: document.documentElement.clientHeight,
            deviceScaleFactor: window.devicePixelRatio,
        }
    }''')
 
    print(dimensions)
    # >>> {'width': 800, 'height': 600, 'deviceScaleFactor': 1}
    await browser.close()
 
asyncio.get_event_loop().run_until_complete(main())
```
### 官方文档
[PyppeteerAPI](https://miyakogi.github.io/pyppeteer/reference.html)