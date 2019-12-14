### How to block nullblank user-agents in IIS 7.5
If request filtering can't handle this, you can try 'URL Rewrite' a free Add-On from Microsoft and pretty helpful anyways.
Create a rule like this:
```Bash
<rule name="NoUserAgent" stopProcessing="true">
    <match url=".*" />
    <conditions>
        <add input="{HTTP_USER_AGENT}" pattern="^$" />
    </conditions>
    <action type="CustomResponse" statusCode="403" statusReason="Forbidden: Access is denied." statusDescription="You did not present a User-Agent header which is required for this site" />
</rule>
```
You can also return a 404 or whatever else you want rather than a 403.
### Blocking Bots Based on User-Agent
open URL Rewrite
Go to 'Add Rule(s)…' in the upper left of the window.
Select 'Request blocking' and click OK
Use the following settings:
```Bash
Block access based on: User-agent Header
Block request that: Matches the Pattern
Pattern (User-agent Header): Googlebot|Ads-Google|msnbot|Altavista|Slurp
Using: Regular Expressions
How to block: Send an HTTP 403 (Forbidden) Response
```