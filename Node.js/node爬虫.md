#1.初始化package.json
执行npm init，初始化package.json文件
#2.安装依赖
express （使用express来搭建一个简单的Http服务器。当然，你也可以使用node中自带的http模块）
superagent （superagent是node里一个非常方便的、轻量的、渐进式的第三方客户端请求代理模块，用他来请求目标页面）
cheerio （cheerio相当于node版的jQuery，用过jQuery的同学会非常容易上手。它主要是用来获取抓取到的页面元素和其中的数据信息）
#根目录新建 index.js
```Javascript
const express = require('express');
const app = express();

let server = app.listen(3000, function () {
  let host = server.address().address;
  let port = server.address().port;
  console.log('Your App is running at http://%s:%s', host, port);
});

const superagent= require('superagent');

let hotNews = [];                                // 热点新闻
let localNews = [];                              // 本地新闻

superagent.get('http://news.baidu.com/guoji').end((err, res) => {
  if (err) {
    console.log(`热点新闻抓取失败 - ${err}`)
  } else {
   hotNews = getHotNews(res)
  }
});

/**
 * index.js
 * [description] - 抓取热点新闻页面
 */
// 引入所需要的第三方包
const cheerio = require('cheerio');

let getHotNews = (res) => {
  let hotNews = [];
  let $ = cheerio.load(res.text);

  // 找到目标数据所在的页面元素，获取数据
  $('#instant-news ul li a').each((idx, ele) => {
    let news = {
      title: $(ele).text(),        // 获取新闻标题
      href: $(ele).attr('href')    // 获取新闻网页链接
    };
    hotNews.push(news)              // 存入最终结果数组
  });
  return hotNews
};

app.get('/', async (req, res, next) => {
  res.send(hotNews);
});
```