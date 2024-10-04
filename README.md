# Traceroute

Simple traceroute realizations - C++ (Windows only) & Go.
C++ CLI was created using winsock2.h and wsws2tcpip.h
~~Go realization in dev :D~~
## Requirements
# C++:
- [CMake][cmake-link]
- Compiler (like [MinGW][compiler-link])
- Windows

# Go:
~~In dev~~
## Installation (C++)
> [!NOTE]
> traceTests.exe can be disabled by using `-DBUILD_TESTS=OFF`\
> Default value: ON
#
```bash
$ git clone https://github.com/Kimishu/traceroute.git
$ mkdir build && cd build
$ cmake -G "MinGW Makefiles" ..
$ cmake --build .
```
Now u can put .exe file in any $PATH folder (or create ur own) and call it from any place.
## Usage
> [!CAUTION]
> After program building recommended to use default CMD (not git bash) to dodge segmentation fault errors.\
> Also u should disable your firewall to see full and correct router path to destination address.\
> If u dont do that, u CAN reach the destination but u CANT see any info about routers.

Help command can be called like this or by providing wrong args :0
```cmd
traceroute.exe --help 
```
And main usage "command" - simple link to destination
```cmd
traceroute.exe google.com
```
## Testing
```cmd
traceTests.exe <any address>
```
[//]: #
   [compiler-link]: <https://github.com/niXman/mingw-builds-binaries?tab=readme-ov-file>
   [cmake-link]: <https://cmake.org/>
