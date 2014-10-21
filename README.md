RSHELL
==========

Licensing information: See LICENSE
---
Source can be downloaded from https://github.com/aorti017/rshell.git
---

Author & Contributor List
----------
Alexander Ortiz

Program Overview
---------
RShell is meant to be a basic implementation of the Bash shell written in C++. Utilizing the execvp function RShell can execute all commands found in ```/bin```.

File List
----------
```
.:

Makefile

LICENSE

README.md

./src

./tests
```
```
./src:

hw0.cpp
```
```
./tests:

exec.script
```

How to run
----------

1. Clone the repository from the link given above
2. ```cd rshell```
3. ```make```
4. ```bin/rshell```

Program Operation
---------
* Multiple commands can be stirnged together on a single line by using the ```&&```, ```||```, or ```;``` connectors.
* Using more than one type of connector per line will result in a ```Multiple connector types forbidden``` error message.
* Excess connectors inserted in the front of a command line, in between commands, or at the end of a command line will be ignored.

Bugs
---------
* Using connectors as part of or the parameter for a command may cause the command to not execute properly.
* "&" and "|" are interpreted as meaning the same thing as the "&&" and "||" connectors when used in place of connectors.
* When using "&&" connectors if any extra (more than 1) of these connectors are inserted in between any two commands and any of the said extra connectors are seperated by a space any command after the excess connectors will not execute.
