# 反向代理的应用：
```Bash
现在许多大型 web 网站都用到反向代理。
除了可以防止外网对内网服务器的恶性攻击、缓存以减少服务器的压力和访问安全控制之外，还可以进行负载均衡，将用户请求分配给多个服务器
```
# 反向代理的好处：
```Bash
(1). 解决了网站服务器对外可见的问题；
(2). 节约了有限的 IP 地址资源，企业内所有的网站共享一个在 internet 中注册的 IP 地址，这些服务器分配私有地址，采用虚拟主机的方式对外提供服务；
(3). 保护了真实的 web 服务器，web 服务器对外不可见，外网只能看到反向代理服务器，而反向代理服务器上并没有真实数据，因此，保证了 web 服务器的资源安全；
(4). 加速了对网站访问速度，减轻 web 服务器的负担，反向代理具有缓存网页的功能，如果用户需要的内容在缓存中，则可以直接从代理服务其中获取，减轻了 web 服务器的负荷，同时也加快了用户的访问速度。
(5).可以实现安全过滤，流控，防 DDOS 等一系列策略
```
# 反向代理的实现
```Bash
1）需要有一个负载均衡设备来分发用户请求，将用户请求分发到空闲的服务器上
2）服务器返回自己的服务到负载均衡设备
3）负载均衡将服务器的服务返回用户
```
# 提示
```Bash
强烈建议不要直接修改 nginx.conf 文件，一般是根据类似如下的代码 include /etc/nginx/conf.d/*.conf;进行对应目录下新建文件的配置，以避免较大的配置影响.
"proxy_pass http://xxxxxx"  作为代理的位置，需要明确自己在哪种条件下进行代理的使用，具体实践需要参考 nginx 配置语法，着重在于正则表达的限制条件
```
# nginx 反向代理 参考配置代码
```Bash
#设定负载均衡的服务器列表
        #weigth参数表示权值，权值越高被分配到的几率越大
        upstream Mos{
            server 192.168.1.106:80 weight=3;  
            server 192.168.1.143:80 weight=1;  
            #server localhost:80 weight=1; #本机
        }
        server {
            listen       80;
            server_name  localhost;
            index index.html index.htm index.php;
            root /mnt/web/hyicnoa;

            location / {
                if (!-e $request_filename){
                    rewrite ^/(.*)$ /index.php?s=/$1 last;
                    }
                proxy_pass http://Mos ;  #在这里设置一个代理，和upstream的名字一样
                }
            location ~ .*\.(php|php5)?$
            {
                #fastcgi_pass  unix:/tmp/php-cgi.sock;
                fastcgi_pass  127.0.0.1:9000;
                fastcgi_index index.php;
                include fastcgi.conf;
            }
            location ~ \.php$ 
            {
                proxy_pass  http://192.168.1.106; #此处限制访问 php 文件时，跳转到192.168.1.106 服务器解析
            }
            location ~ .*\.(js|css)?$
            {
            expires 1h;
            }
        }
```
# 根据配置信息(当访问php文件时会由192.168.1.106服务器解析内容,完成后再交由192.168.1.87:80显示给用户)
```Bash
location ~ \.php$
{
proxy_pass  http://192.168.1.106; #此处限制访问php文件时，跳转到192.168.1.106服务器解析
}
```
# 根据不同目录
```Bash
location /zmPro/ {
        proxy_pass http://192.168.1.143; #当访问zmPro目录时,由该IP服务器进行解析
}
```
![配置截图](https://upload-images.jianshu.io/upload_images/2578557-c0ce1604907470a7?imageMogr2/auto-orient/strip%7CimageView2/2/w/659/format/webp)
## 结果描述
```Bash
访问 html 文件时，会根据权重由 192.168.1.106 和 192.168.1.143 进行解析
访问 php 文件时，只会由 192.168.1.106 解析
访问 zmPro 目录下的文件时，会由 192.168.1.143 解析，此时如果是 html 文件，如果 192.168.1.106 对应位置没有相关文件，会根据前面的权重出现不同的访问状况，一种情况是由  192.168.1.143 正常解析，另一种情况是 192.168.1.106 无对应文件而 404 解析失败
```