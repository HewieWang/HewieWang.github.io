### Blocking Bots Based on User-Agent
On Apache servers it is very easy to block unwanted bots using the .htaccess file. Simply add the following code to the file to block the engines.
```Bash
RewriteEngine On
RewriteCond %{HTTP_USER_AGENT} Googlebot [OR]
RewriteCond %{HTTP_USER_AGENT} AdsBot-Google [OR]
RewriteCond %{HTTP_USER_AGENT} msnbot [OR]
RewriteCond %{HTTP_USER_AGENT} AltaVista [OR]
RewriteCond %{HTTP_USER_AGENT} Slurp
RewriteRule . - [F,L]
```
The following code will return a 403 error to Google
If you would prefer to redirect the bots to a specific page then just replace the last line above with the following.
```Bash
RewriteRule ^.*$ "http\:\/\/www.yoursite\.com\/nobots.html" [R=301,L]
```