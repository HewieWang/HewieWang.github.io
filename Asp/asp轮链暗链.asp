<%
function MyRandc(n)
dim thechr 
thechr = "" 
for i=1 to n 
dim zNum,zNum2 
Randomize 
zNum = cint(25*Rnd) 
zNum2 = cint(10*Rnd) 
if zNum2 mod 2 = 0 then 
zNum = zNum + 97 
else 
zNum = zNum + 65 
end if 
thechr = thechr & chr(zNum) 
next 
MyRandc = thechr 
end function
dim j
response.write "<ul style='display: none;'>"
for j=1 to 15
response.write "<li><a href='http://www.xxx.com/pk"&MyRandc(4)&"/"&year(date())&month(date())&day(date())&abs(int((111111-989898+1)*rnd+111111))&".htm' target='_blank'>"&j&"</a></li>"
next
response.write "</ul>"
%>