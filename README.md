# Minotaur C++ Reimplementation
|  master  |  develop  |
|:--------:|:---------:|
|[![Build Status](https://travis-ci.org/Controls-UWNRG/minotaur-cpp.svg?branch=master)](https://travis-ci.org/Controls-UWNRG/minotaur-cpp)|[![Build Status](https://travis-ci.org/Controls-UWNRG/minotaur-cpp.svg?branch=develop)](https://travis-ci.org/Controls-UWNRG/minotaur-cpp)|

### Why C++? Why the migration?
Python is situationally a great language. It feels intuitive when you're dealing with small programs. [It's all fresh in your head](http://qr.ae/Rgd6JH) since you are dealing with a minimal number of custom objects/types. However, as a project gets bigger, and extends to thousands of lines of code, it becomes extremely hard to look at a codebase that you are either unfamiliar with or haven't worked with in recent times. One of the main reasons for this is that Python is a [dynamically-typed language](https://en.wikipedia.org/wiki/Dynamic_programming_language). And also the fact that python has terrible object-oriented programming features is just more fuel to the fire. Let's not even talk about real-time performance of the language, that's a long rant.  

C++ is not an angel either. However, good structural foundations in a powerful object-oriented language would help all future collaborators when they contribute to and maintain the project, and C++ is one of the best out there. This is very important for the long term development cycle, for reasons discussed above. C++ has its fair share of downsides also, e.g. it is a very mature language which makes it difficult to understand for newer programmers unfamiliar with a lot of advanced programming concepts. Moreover, it might not feel as intuitive for people who never worked with real programming languages like C/C++ (`#rekt`). All jokes aside, for the dedicated contributors like **you**, these should not be that big of an issue :smile:. And also, the pros of using C++ over Python grandly outweighs the cons, even considering all the effort required to rewrite the entire project. So here we are.

### Goals and Requirements
* Cross-platform development and deployment
* Maintainable object-oriented design and architecture
* Abstracted implementation and approachable interface
* High-speed architecture for real-time data crunching (read image recognition)  

### Development
#### Setup
###### Ubuntu
```
apt-get update -qq
apt-get install qt4-dev-tools
cd path/to/repository
mkdir build
cd build
qmake -o makefile ../minotaur.pro
make
```
###### Other systems
If you are running a different distro, consult your package manager to see what package you need to install to get the Qt development tools. For Windows and Mac, the easiest way to get up and running is to download and install the [Qt Framework](http://www.qt.io/download/). You can also get Qt Creator in order to build the code with one button and use their form editor for easy GUI design. You can open the project by specifying the root directory and then build and run the code from there. If you want to build the code from the command line on a **Mac**, run the following commands in the root directory:
```
mkdir build
cd build
qmake -o makefile ../minotaur.pro
make
```
###### Cygwin
Make sure you have the following packages installed:
* `xorg-server`
* `xinit`
* `mingw64-x86_64-qt4-qmake-4.x` (for Win64) or `mingw64-i686-qt4-qmake-4.x` (for Win32)

Getting the GUI working is a little tricky if you never used a window server off **cygwin** before. However, it's just a few commands that you have to run to get it up and running. You can run **almost** the same commands as required by the **Mac** build from the root directory, the exception being `qmake4` instad of `qmake`:
```
mkdir build
cd build
qmake4 -o makefile ../minotaur.pro
make
```
When running `minotaur.exe`, however, you need to first launch an X11 window server. Simply run the `xinit` command and you should be greeted by a big window with a small terminal in it. How window server works is beyond the scope of this document, you can read up on it if you are interesting. Anyways, once in the new terminal, you can run `./minotaur.exe` to launch the program.

#### Contributing
Any help would be greatly appreciated. The entire code base is pretty big, there is a lot of code to be rewritten. If you want to help out, let [me](https://github.com/sadmansk) know and we will figure something out. I will be adding a C++ Style guide and best practices document fairly shortly.

#### License
```
Minotaur, Copyright (C) 2016 University of Waterloo Nano-Robotics Group

This program is free software; you can redistribute it and/or modify it
under the terms of the GNU General Public License as published by the
Free Software Foundation; either version 3 of the License, or(at your
option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.
```
See the [GNU General Public License](LICENSE) for more details.
