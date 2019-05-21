### Apache
```Python
ProxyPassMatch    ^/media http://localhost:9002/(.*)
ProxyPassReverse  ^/media http://localhost:9002/(.*)
```