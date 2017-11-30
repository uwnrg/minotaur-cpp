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

# Setting up Minotaur on Windows
These will describe the basic steps for setting up an environment on Windows.
Things may vary from PC to PC, though.

### Installing Qt
1. Download the Qt community edition and install a version at least 5.6
2. I recommend 5.6 or 5.7 since the later versions take up much more space
3. After installation completes set the `QT_DIR` environment variable, which would look
something like `C:\Qt\%VERSION%\bin`.
4. Add the some variable to `PATH`

### Installing Python
1. Download and install Python 3.6
2. You might want to add the python `include` and `lib` directories to your `PATH`

### Running with CLion
I recommend using CLion, since it's more self contained and does work for you.

1. Download and install CLion, which is free for students
2. If you don't have the Microsoft Visual Studio compiler on your machine
you can either install MVS (yikes!) or follow these steps
https://www.jetbrains.com/help/clion/quick-tutorial-on-configuring-clion-on-windows.html
3. Make sure you install a relatively recent compiler version

## Building Minotaur
1. `git clone https://github.com/uwnrg/minotaur-cpp.git`
2. Open the project in CLion by clicking `Import from sources`
3. CLion will attempt to detect a compiler
4. CLion will attempt to configure the cmake project
5. If that goes well, you can build the project
6. If you get any errors, ping me `@jeffniu22` on Slack

# Setting up Minotaur on MacOS
These will describe the basic steps for setting up an environment on MacOS.
There may be issues if multiple versions of Python are installed on the system.

### Installing Qt
1. Download the Qt community edition and install a version at least 5.6
2. I recommend 5.6 or 5.7 since the later versions take up much more space

### Installing Python
1. You can either download Python 3.6 from the website and install or
2. Use `brew install python3`
3. Verify the python version (minimum 3.4) with `python3 --version`

## Building Minotaur
Here you can either install CLion and import the project (recommended) or
1. `git clone https://github.com/uwnrg/minotaur-cpp.git`
2. `cd minotaur-cpp`
3. `mkdir build`
4. `cd build`
5. `cmake ../ -DCMAKE_PREFIX_PATH=/path/to/qt/%VERSION%/clang_64/lib/cmake`
6. `make`

And then run with `./minotaur-cpp`. Note that if you're using CLion, go to 
`Preference -> Build, Execution, Deployment -> CMake` and add the same `CMAKE_PREFIX_PATH`
argument.

CMake might have a hard time finding the correct Python version because Apple. Ping me `@jeffniu22`
if you've got any issues.

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
