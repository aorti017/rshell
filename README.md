RSHELL
==========

Licensing information: BSD3
---

Program Overview
---------
RShell is meant to be a basic implementation of the Bash shell written in C++. Utilizing the execvp function RShell can execute commands found in ```/bin```.

How to run
----------

1. ```git clone https://github.com/aorti017/rshell.git```
2. ```cd rshell```
3. ```make```
4. ```bin/rshell``` or  ```bin/ls```

Using RShell
---------
* Multiple commands can be stirnged together on a single line by using the ```&&```, ```||```, or ```;``` connectors.
* Connecting multiple commands by ```&&``` will cause commands to execute only if the command prior was successful.
* Connecting multiple commands by ```||``` will cause commands to execute only if the command prior was not successful.
* Connecting multiple commands by ```;``` will cause command to execute regardless of if the prior command was successful.
* Using more than one type of connector per line will result in a ```Multiple connector types forbidden``` error message.
* Excess connectors inserted in the front of a command line, in between commands, or at the end of a command line will be ignored.
* Comments can be inserted using ```#```, anything after ```#``` will be interpreted as a comment.
* The ```exit``` command will exit RShell upon execution.
* The ```cp``` command can be used to copy  the contents of a source file into a destination file.
* ```cp``` can be used with the ```-a``` flag, which will copy using all methods. If no flag is specified it will be copied
  using the fastest method.
* ```cp``` will output an error if the destination file already exists, or if either the source or destination are directories.
* The ```bin/ls``` command can be used with any combination of, or none of, the optional flags ```-a```, ```-l```, or ```-R```.
* ```-a``` displays all files including those that are hidden.
* ```-l``` displays all of the information associated with a file.
* ```-R``` recursively outputs the contents of all the directories in the starting directory.
* When using the ```bin/ls``` command directories are displayed in blue, executables in green, and hidden files with a grey background.
* Input and output can be redirected using ```<```, ```>```, and ```>>```.
* Multiple consecutive instances of ```<``` will only input the last file.
* Multiple consecutive instances of ```>``` or ```>>``` will clear all files then output to the last one.
* Strings can be passed to commands via redirection using ```<<<```.
* Filed descriptors can be specified before the output operator ```>```, such as ```1>```. When doing this there must be a space
  between the command and the file descriptor.
* Commands and redirections can be piped using ```|```.
* Commands beginning with the ```|```, or any other redirector, character will result in an error.
* The ```cd``` command can be used to change the current working directory, which will be displayed in the prompt.
* The ```^C``` signal can be used to kill the current foreground process.
* If there is no current foreground porcess the ```^C``` signal will do nothing.

Bugs/Limitations
---------
* Does not support the functionality of quotation marks with commands like ```echo```.
* Using connectors as part of or the parameter for a command may cause the command to not execute properly, if at all.
* When using ```&&``` connectors if any extra (more than 1) of these connectors are inserted in between any two commands, or in the front of the command line, and any of the said extra connectors are separated by a space any command after the excess connectors will not execute.
* ```&``` is interpreted as ```&&```.
* ```bin/ls``` can not be used with regular expressions.
* When passing a hidden file to ```bin/ls``` the background is not colored grey.
* When running ```cat``` with string redirection, the string is output with no new line.
* If using a custom file descriptor and the file descriptor is bad nothing will be output to either file or stdout.
* If the file ```file``` has contents and the command ```cat<file>>file``` is executed an infinite loop will occur.
* Can not link together piping operations using connectors.
* ```2>``` and ```2>>``` can not combined with pipes.
* ```cd``` is not compatable with regular expressions.
* When trying to run an executable that has the same name as one that can be found with in any of the ```PATH``` directories, the one in the  ```PATH``` directory will be ran instead.
