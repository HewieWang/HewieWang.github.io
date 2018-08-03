On Error Resume Next 

Dim wsh,ye 

set wsh=createobject("wscript.shell") 

wscript.sleep 2000 

for i=1 to 30000

wscript.sleep 8

wsh.sendKeys "^v" 

wsh.sendKeys i 

wsh.sendKeys "%s" 

next 

wscript.quit