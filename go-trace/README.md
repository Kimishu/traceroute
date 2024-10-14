# go-trace

Simple Golang traceroute implementation

## Requirements
- [Compiler][compiler-link]

## Installation

```bash
go install github.com/Kimishu/traceroute/go-trace
```
File will be located in $GOPATH/bin directory
### UNIX
```bash
ls $GOPATH/bin
```
### Windows
```cmd
dir %GOPATH%\bin
```
## Usage
> [!WARNING]
> After installation recommended to disable the firewall to see full and correct router path to destination address.\
> If u dont do that, u CAN reach the destination but u CAN'T see any intermediate routers.

Help command can be called like this or with `-h`
```cmd
go-trace --help 
```
And main usage "command" - simple link to destination
```cmd
go-trace google.com
```
## Conclusion
> [!NOTE]
> Some routers may not send back the message. However, the problem is not on your side. It's fine. \
> In this case, as mentioned above in the situation with a working [firewall](#usage), we will still reach the destination.

[//]: #
   [compiler-link]: <https://go.dev/doc/install>
   [cmake-link]: <https://cmake.org/>