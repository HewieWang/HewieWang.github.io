### Add the code in server
```Bash
if ($http_user_agent ~* (Python|MJ12bot|Trident|GoogleBot)) {
	return 404;
}
```
### Nginx Block parameter ,put the code below before location
```Bash
error_page 418 = @blockAccess;
if ($args ~* "eval") {
    return 418;
}
location @blockAccess {
    deny all;
}
```