## IIS的文件防盗链处理
#### 首先安装官方的URL Rewrite Module（URL 重写模块），之后配置检测referer 的规则即可。配置有两种办法，一个是在IIS 里找到这个URL 重写模块，通过可视化操作设置规则。另一个办法是直接在网站根目录的web.config 里添加规则。我用的是后一种办法。在web.config 里添加一条规则：
```Bash
<rule name="Prevent hotlinking">
    <match url="^.*\.(jpg|png|gif)$" ignoreCase="true" />
    <conditions>
        <add input="{HTTP_REFERER}" pattern="https://simple.com/.*" negate="true" />
    </conditions>
    <action type="CustomResponse" statusCode="403" />
</rule>
```
#### 如果最后你不想显示403状态码，而是显示一个自定义图片的话，可以把action 改成这样：
```Bash
<action type="Rewrite" url="/img/bitch.png" />
```
#### 上面的代码只写了规则部分，如果你只有这一条规则，那么web.config 里的代码是这样的（这涉及到web.config 的格式规范）：
```Bash
<?xml version="1.0" encoding="UTF-8"?>
<configuration>
    <system.webServer>
        <rewrite>
            <rules>
                <rule name="Prevent hotlinking">
                    <match url="^.*\.(jpg|png|gif)$" ignoreCase="true" />
                    <conditions>
                        <add input="{HTTP_REFERER}" pattern="https://simple.com/.*" negate="true" />
                    </conditions>
                    <action type="CustomResponse" statusCode="403" />
                </rule>
            </rules>
        </rewrite>
        <httpErrors errorMode="DetailedLocalOnly" />
    </system.webServer>
</configuration>
```