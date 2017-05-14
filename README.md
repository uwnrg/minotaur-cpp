# Minotaur C++ Reimplementation
|  master  |  develop  |
|:--------:|:---------:|
|[![Build Status](https://travis-ci.org/uwnrg/minotaur-cpp.svg?branch=master)](https://travis-ci.org/uwnrg/minotaur-cpp)|[![Build Status](https://travis-ci.org/uwnrg/minotaur-cpp.svg?branch=develop)](https://travis-ci.org/uwnrg/minotaur-cpp)|

### Table of Contents

* [Introduction](#introduction)
* [Setup](#setup)
  - [Ubuntu](#ubuntu)
  - [Mac](#mac)
  - [Other systems](#other-systems)
  - [Cygwin](#cygwin)
* [Contributing](#contributing)
* [License](#license)

### Introduction
Minotaur is the controls software for our microrobots. It is being actively developed
with the following goals in mind:

* Cross-platform development and deployment
* Maintainable object-oriented design and architecture
* Abstracted implementation and approachable interface
* High-speed architecture for real-time data crunching (read image recognition)
* Expose controller API that listens to Python scripts for quick navigation deployment

Please checkout the *Projects* tab for information about roadmap and current progress.

### Setup

#### Ubuntu
```
apt-get update -qq
apt-get install qt4-dev-tools
cd path/to/repository
mkdir build
cd build
qmake -o makefile ../minotaur.pro
make
```

#### Mac

For **Mac** the easiest way to get up and running is to download and install the
[Qt Framework](http://www.qt.io/download/). You can also get Qt Creator in order
to build the code with one button and use their form editor for easy GUI design.
You can open the project by specifying the root directory and then build and run
the code from there. To build the code, run the following commands in the root
directory:

```
mkdir build
cd build
qmake -spec macx-g++ ../minotaur.pro
make
```
The application will be in `/builds/` and named `minotaur.app`. Run it from the
Finder since **Mac** treats applications like directories.

#### Other systems

If you are running a different linux distro, consult your package manager to see
what package you need to install to get the Qt development tools. For Windows (and
Mac), the easiest way to get up and running is to download and install the
[Qt Framework](http://www.qt.io/download/). You can also get Qt Creator in order
to build the code with one button and use their form editor for easy GUI design.
You can open the project by specifying the root directory and then build and run
the code from there.

**Visual Studio**: If you want to develop on Visual Studio and don't already have
Qt setup there, (for VS 2013 and older) download the Qt Visual Studio Add-in from
[here](http://download.qt.io/official_releases/vsaddin/). If you are using **Visual
Studio 2015** or above, you need to download this
[package](https://visualstudiogallery.msdn.microsoft.com/c89ff880-8509-47a4-a262-e4fa07168408)
instead. Then restart Visual Studio, click on the Qt menu, go to options and add
your Qt versions.

Once you have Qt setup, go to `QT5`->`Open Qt Project File (.pro)` and open `minotaur.pro` from this repository and you are all set!

#### Cygwin
Make sure you have the following packages installed:
* `xorg-server`
* `xinit`
* `mingw64-x86_64-qt4-qmake-4.x` (for Win64) or `mingw64-i686-qt4-qmake-4.x`
    (for Win32)

Getting the GUI working is a little tricky if you never used a window server off
**cygwin** before. However, it's just a few commands that you have to run to get
it up and running. You can run **almost** the same commands as required by the
**Mac** build from the root directory, the exception being `qmake4` instad of
`qmake`:

```
mkdir build
cd build
qmake4 -o makefile ../minotaur.pro
make
```

When running `minotaur.exe`, however, you need to first launch an X11 window server.
Simply run the `xinit` command and you should be greeted by a big window with a
small terminal in it. How window server works is beyond the scope of this document,
you can read up on it if you are interesting. Anyways, once in the new terminal,
you can run `./minotaur.exe` to launch the program.

### Contributing
Please refer to the [Contributing Guidelines](CONTRIBUTING.md).

### License
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
