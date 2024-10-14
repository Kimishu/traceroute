# traceroute

Simple C++ traceroute implementation
>[!NOTE]
>C++ CLI was created using winsock2.h and wsws2tcpip.h

## Requirements
- [CMake][cmake-link]
- Compiler (like [MinGW][compiler-link])
- Windows

## Installation
> [!TIP]
> traceTests.exe can be disabled by using `-DBUILD_TESTS=OFF`\
> Default value: ON
```bash
git clone https://github.com/Kimishu/traceroute.git
```
```bash
mkdir build && cd build
```
```bash
cmake -G "MinGW Makefiles" ..
```
```bash
cmake --build .
```
Now u can put `.exe` file in any $PATH folder (or create ur own) and call it from any place.
## Usage
> [!WARNING]
> After program building recommended to disable your firewall to see full and correct router path to destination address.\
> If u dont do that, u CAN reach the destination but u CAN'T see any intermediate routers.

Help command can be called like this or by providing wrong args :0
```cmd
traceroute --help 
```
And main usage "command" - simple link to destination
```cmd
traceroute google.com
```
## Testing
```cmd
traceTests <any address>
```
## Conclusion
> [!NOTE]
> Some routers may not send back the message. However, the problem is not on your side. It's fine. \
> In this case, as mentioned above in the situation with a working [firewall](#usage), we will still reach the destination.

[//]: #
   [compiler-link]: <https://github.com/niXman/mingw-builds-binaries?tab=readme-ov-file>
   [cmake-link]: <https://cmake.org/>
