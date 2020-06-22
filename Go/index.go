package main

import (
	
    "net/http"
)

var headerhtml=`
<!DOCTYPE html>
<html lang=en>
<head>
    <meta charset="utf-8">
    <meta http-equiv="X-UA-Compatible" content="IE=edge">
    <meta name="HandheldFriendly" content="True">
    <meta name="viewport" content="width=device-width, initial-scale=1, maximum-scale=1" />
    <meta name="description" content="phith0n的小站，长期存在与分享关于网络安全与各种编程的原创文章。">
    <meta name="keywords" content="phith0n,网络安全,代码审计,信息安全,漏洞挖掘,php,C++,mysql,python">
    <link rel="shortcut icon" href="https://www.leavesongs.com/static/cactus/images/favicon.ico">
    <link rel="icon" type="image/png" href="https://www.leavesongs.com/static/cactus/images/favicon-192x192.png" sizes="192x192">
    <link rel="apple-touch-icon" sizes="180x180" href="https://www.leavesongs.com/static/cactus/images/apple-touch-icon.png">
    <title>首页 | 离别歌</title>
    <link rel="stylesheet" href="https://www.leavesongs.com/static/cactus/lib/font-awesome/css/font-awesome.min.css">
    <link rel="stylesheet" href="https://www.leavesongs.com/static/cactus/lib/meslo-LG/styles.css">
    <link rel="stylesheet" href="https://www.leavesongs.com/static/cactus/css/style.css">
    <link rel="stylesheet" href="https://cdn.jsdelivr.net/npm/flexboxgrid@6.3.1/dist/flexboxgrid.min.css" integrity="sha256-/8+sU56ayrJGahG9mmcUaNEghbavaceGybkdqO96Kk0=" crossorigin="anonymous">
    <link rel="alternate" type="application/atom+xml" href="/feed/" title="离别歌" />
    <script src="https://www.leavesongs.com/static/cactus/lib/jquery/jquery.min.js"></script>
<link rel="stylesheet" href="https://www.leavesongs.com/static/cactus/css/button.css">
</head>
<body>
<div class="content index width mx-auto px2 my4">
<header id="header">
  <a href="#">
    <div id="logo" style="background-image: url('https://www.leavesongs.com/static/cactus/images/logo.png');"></div>
    <div id="title">
      <h1>离别歌</h1>
    </div>
  </a>
  <div id="nav">
    <ul>
      <li class="icon">
        <a href="#"><i class="fa fa-bars fa-2x"></i></a>
      </li>
        <li><a href="/" title="" >首页</a></li>
        <li><a href="/list" title="" >文章</a></li>
        <li><a href="/other/tinger.html" title="" >关于</a></li>
        <li><a href="/other/friends-link.html" title="友情链接" >友链</a></li>
        <li><a href="" title="Phithon的开源项目" target="_blank">项目</a></li></ul>
  </div>
</header>
`
var foothtml=`
<footer id="footer">
  <div class="footer-left">
    Copyright &copy; 2020 Powered by talkbook
  </div>
  <div class="footer-right">
    <nav>
      <ul>
          <li><a href="/">首页</a></li>
          <li><a href="http://weibo.com/101yx" target="_blank">Telegram</a></li>
          <li><a href="https://github.com/phith0n" target="_blank">Github</a></li>
      </ul>
    </nav>
  </div>
</footer>
<script src="https://www.leavesongs.com/static/cactus/js/main.js"></script>
</body>
</html>
`
func SayHello(w http.ResponseWriter, req *http.Request) {
    w.Write([]byte("Hello"))
}
func Archives(w http.ResponseWriter, req *http.Request) {
    w.Write([]byte(headerhtml+`
111
`+foothtml))
}
func Index(w http.ResponseWriter, req *http.Request) {
	w.Header().Set("Content-Type", "text/html; charset=utf-8")
	// w.Write([]byte(req.RemoteAddr))
    w.Write([]byte(headerhtml+`
    <section id="about">
        <p>随便写写</p>
    </section>
    <section id="search">
        <span class="h1"><a href="#">Search</a></span>
        <form action="/search/" method="get">
        <div class="row">
            <div class="col-xs-8 col-md-4">
                <input title="关键词" type="text" name="keyword" value="">
            </div>
            <div class="col-xs-4 col-md-2">
                <button type="submit" class="ui-button ui-button-sgreen" >&nbsp;搜索&nbsp;</button>
            </div>
        </div>
        </form>
    </section>
    <section id="writing">
        <span class="h1"><a href="#">Archives</a></span>
        <ul class="post-list">
            
            <li class="post-item">
                <div class="meta">
                    
                    <time datetime="March 12, 2020, 9:41 p.m." itemprop="datePublished">
                        2020 Mar 12 
                    </time>
                    
                </div>
                <span>
                    <a href="/PENETRATION/thinking-about-config-file-arbitrary-write.html" >经典写配置漏洞与几种变形</a>
                </span>
            </li>
            
            <li class="post-item">
                <div class="meta">
                    
                    <time datetime="Feb. 19, 2020, 11:45 p.m." itemprop="datePublished">
                        2020 Feb 19 
                    </time>
                    
                </div>
                <span>
                    <a href="/PENETRATION/linux-suid-privilege-escalation.html" >谈一谈Linux与suid提权</a>
                </span>
            </li>
            
            <li class="post-item">
                <div class="meta">
                    
                    <time datetime="Jan. 23, 2020, 7:14 p.m." itemprop="datePublished">
                        2020 Jan 23 
                    </time>
                    
                </div>
                <span>
                    <a href="/THINK/talk-about-2019.html" >2019年的一部分总结</a>
                </span>
            </li>
            
            <li class="post-item">
                <div class="meta">
                    
                    <time datetime="Dec. 18, 2019, 10:19 p.m." itemprop="datePublished">
                        2019 Dec 18 
                    </time>
                    
                </div>
                <span>
                    <a href="/PENETRATION/dynamic-features-and-webshell-tricks-in-php.html" >PHP动态特性的捕捉与逃逸</a>
                </span>
            </li>
            
            <li class="post-item">
                <div class="meta">
                    
                    <time datetime="Oct. 22, 2019, 11:22 a.m." itemprop="datePublished">
                        2019 Oct 22 
                    </time>
                    
                </div>
                <span>
                    <a href="/SHARE/xray-introduce.html" >XRay旅行记 - 从内部项目到社区项目的蜕变</a>
                </span>
            </li>
            
            <li class="post-item">
                <div class="meta">
                    
                    <time datetime="Oct. 14, 2019, 11:49 a.m." itemprop="datePublished">
                        2019 Oct 14 
                    </time>
                    
                </div>
                <span>
                    <a href="/PENETRATION/java-security-travel-6.html" target="_blank">Java安全漫谈 - 06.RMI篇(3)</a>
                </span>
            </li>
            
            <li class="post-item">
                <div class="meta">
                    
                    <time datetime="Sept. 18, 2019, 11:49 a.m." itemprop="datePublished">
                        2019 Sep 18 
                    </time>
                    
                </div>
                <span>
                    <a href="/PENETRATION/attack-scrapy.html" >攻击Scrapyd爬虫</a>
                </span>
            </li>
            
            <li class="post-item">
                <div class="meta">
                    
                    <time datetime="Aug. 2, 2019, 9:45 p.m." itemprop="datePublished">
                        2019 Aug 02 
                    </time>
                    
                </div>
                <span>
                    <a href="/PENETRATION/django-jsonfield-cve-2019-14234.html" >Django JSONField SQL注入漏洞（CVE-2019-14234）分析与影响</a>
                </span>
            </li>
            
            <li class="post-item">
                <div class="meta">
                    
                    <time datetime="June 29, 2019, 7:47 p.m." itemprop="datePublished">
                        2019 Jun 29 
                    </time>
                    
                </div>
                <span>
                    <a href="/PENETRATION/when-imagemagick-meet-getimagesize.html" >imagemagick邂逅getimagesize的那点事儿</a>
                </span>
            </li>
            
            <li class="post-item">
                <div class="meta">
                    
                    <time datetime="May 27, 2019, 10:57 a.m." itemprop="datePublished">
                        2019 May 27 
                    </time>
                    
                </div>
                <span>
                    <a href="/PENETRATION/code-breaking-2018-python-sandbox.html" >Code-Breaking中的两个Python沙箱</a>
                </span>
            </li>
            
        </ul>
    </section>
</div>
`+foothtml))
}
func main() {
    http.HandleFunc("/", Index)
    http.HandleFunc("/list", Archives)
    http.ListenAndServe(":8001", nil)
}
