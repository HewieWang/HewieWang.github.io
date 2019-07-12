### cmd_powershell_create_user
```Bash
Syntax of creating new users and grant them admin right (Powershell & CMD)

Bash

cmd.exe < run Administrator
net user /add [Username] [Password]
new localgroup Administrators [Username] /add

Powershell

NET USER [Username] [Password]/add /y /expires:never
NET LOCALGROUP Administrators [Username]/add
WMIC USERACCOUNT WHERE "Name='[Username]'" SET PasswordExpires=FALSE
```