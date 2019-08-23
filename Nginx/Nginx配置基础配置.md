### server
```Batch
listen       80;
        listen 443 ssl;
        server_name xxx.com;

        ssl_certificate /root/bot/full_chain.pem;
        ssl_certificate_key /root/bot/private.key;

        #charset koi8-r;

        #access_log  logs/host.access.log  main;

        location / {
                 try_files $uri @gunicorn_proxy;
         }

         location @gunicorn_proxy{
                 proxy_set_header X_Forwarded-For $proxy_add_x_forwarded_for;
                 proxy_set_header Host $host;
                 proxy_redirect off;
                 proxy_pass http://xxx.com:8090;
        }
```
#### https://www.jianshu.com/p/be9dd421fb8d