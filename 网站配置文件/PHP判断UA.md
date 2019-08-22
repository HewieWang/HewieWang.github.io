### 判断蜘蛛访问的代码
```Batch
//蜘蛛ua特征码
$bots = array
(
	  'Google Bot' => 'googlebot'
	, 'Google Bot' => 'google'
	, 'MSN' => 'msnbot'
	, 'Alex' => 'ia_archiver'
	, 'Lycos' => 'lycos'
	, 'Ask Jeeves' => 'jeeves'
	, 'Altavista' => 'scooter'
	, 'AllTheWeb' => 'fast-webcrawler'
	, 'Inktomi' => 'slurp@inktomi'
	, 'Turnitin.com' => 'turnitinbot'
	, 'Technorati' => 'technorati'
	, 'Yahoo' => 'yahoo'
	, 'Findexa' => 'findexa'
	, 'NextLinks' => 'findlinks'
	, 'Gais' => 'gaisbo'
	, 'WiseNut' => 'zyborg'
	, 'WhoisSource' => 'surveybot'
	, 'Bloglines' => 'bloglines'
	, 'BlogSearch' => 'blogsearch'
	, 'PubSub' => 'pubsub'
	, 'Syndic8' => 'syndic8'
	, 'RadioUserland' => 'userland'
	, 'Gigabot' => 'gigabot'
	, 'Become.com' => 'become.com'
	, 'Baidu' => 'baiduspider'
	, 'so.com' => '360spider'
	, 'Sogou' => 'spider'
	, 'soso.com' => 'sosospider'
	, 'Yandex' => 'yandex'
);
$useragent = $_SERVER['HTTP_USER_AGENT'];
foreach ( $bots as $name => $lookfor ) {
	if ( stristr( $useragent, $lookfor ) !== false ) {
		$is_robot = true;
		break;
	}
}
```
## – [蜘蛛UA列表](http://user-agent-string.info/list-of-ua/bots)
## – [浏览器UA列表](http://user-agent-string.info/list-of-ua)