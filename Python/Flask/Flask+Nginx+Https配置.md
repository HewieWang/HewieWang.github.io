```Python
from flask import Flask
 
app = Flask(__name__)
 
@app.route('/')
def index():
    return "Hello"
 
if __name__=="__main__":
    app.run(host="0.0.0.0", port=8090)
```
### 绑定域名
```Python
server  {
         listen 80;

         server_name xxx.cn;

         location / {
                 try_files $uri @gunicorn_proxy;
         }

         location @gunicorn_proxy{
                 proxy_set_header X_Forwarded-For $proxy_add_x_forwarded_for;
                 proxy_set_header Host $host;
                 proxy_redirect off;
                 proxy_pass http://127.0.0.1:4359;
         }
}
```
### Https配置
```Python
server {
        listen 443 ssl;
        server_name xxx.cn;
        charset utf-8;

        ssl_certificate /root/tech.pem;
        ssl_certificate_key /root/tech.key;
        location / {
                 try_files $uri @gunicorn_proxy;
        }

        location @gunicorn_proxy{
                 proxy_set_header X_Forwarded-For $proxy_add_x_forwarded_for;
                 proxy_set_header Host $host;
                 proxy_redirect off;
                 proxy_pass http://127.0.0.1:4359;
        }
    }
```
### 强制HTTPS
```Python
if ($server_port !~ 443){
        rewrite ^(/.*)$ https://$host$1 permanent;
}
```