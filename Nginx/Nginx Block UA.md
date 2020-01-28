### Add the code in server
```Bash
if ($http_user_agent ~* (Python|MJ12bot|Trident|GoogleBot)) {
	return 404;
}
```