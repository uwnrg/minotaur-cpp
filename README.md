# Minotaur C++ Reimplementation
|  master  |  develop  |
|:--------:|:---------:|
|[![Build Status](https://travis-ci.org/uwnrg/minotaur-cpp.svg?branch=master)](https://travis-ci.org/uwnrg/minotaur-cpp)|[![Build Status](https://travis-ci.org/uwnrg/minotaur-cpp.svg?branch=develop)](https://travis-ci.org/uwnrg/minotaur-cpp)|

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
These will describe the steps for setting up an environment on Windows.
This has been tested on a clean installation of Windows 10.

1. Download and install Git `https://git-scm.com/download/win`
2. Download and install Python 3.6 `https://www.python.org/downloads`
    - Select `Add Python 3.6 to PATH`
    - Under `Customize installation` ensure `Add Python to environment variables` and
    `Precompiled standard library` are checked
3. Download and install Qt `https://www1.qt.io/download-open-source`
    - Choose only Qt 5.6 for `MinGW 4.9.2 32 bit`
    - Qt ships with `g++.exe` and `mingw32-make.exe`
4. Download and install CMake `https://cmake.org/download`
    - Make sure to check `Add CMake to the system PATH`
5. Add the Qt tools directory to the <strong>System</strong> PATH: `C:/Qt/tools/mingw492_32/bin`
    - Make sure to restart PowerShell or cmd
6. Add the Qt 5.6 binaries to the <strong>System</strong> PATH: `C:/Qt/5.6/mingw49_32/bin`
7. Clone the repo `git clone https://github.com/uwnrg/minotaur-cpp.git`
    - `cd minotaur-cpp`
    - `mkdir build`
    - `cd build`
8. The following variables are needed by `cmake`
    - `CMAKE_PREFIX_PATH` points to Qt: `C:/Qt/5.6/mingw49_32/lib/cmake`
    - `PYTHON_INCLUDE_DIRS` points to Python headers: `C:/Python36-32/include`
    - `PYTHON_LIBRARY` points to Python library: `C:/Python36-32/libs/python36.lib`

### Building Minotaur

9. `cmake .. -G "MinGW Makefiles" -DCMAKE_PREFIX_PATH="C:/Qt/5.6/mingw49_32/lib/cmake" -DPYTHON_INCLUDE_DIRS="C:/Python36-32/include" -DPYTHON_LIBRARY="C:/Python36-32/libs/python36.lib"`
10. `mingw32-make`
11. Double-click `minotaur-cpp.exe`

### Post-setup
1. You can add the Python `include` and `lib` directories to your <strong>System</strong> PATH or
2. If you are using CLion, you can add `CMAKE_PREFIX_PATH`, `PYTHON_INCLUDE_DIRS` and `PYTHON_LIBRARY`
as either environment variables or `cmake` flags
3. If CLion can't find `g++` you can point it directly to the location in `Build, Execution, and Deployment`
4. You can also set the `QTDIR` environment variable to `C:/Qt/5.6`
5. You should now be able to build and run with CLion or `cmake .. -G "MinGW Makefiles"` and `mingw32-make`

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

### Installing OpenCV 3
1. OpenCV 3 is the required version
2. Build and install with `brew install opencv`
3. Additional steps or dependencies (such as Python 2 and Python 3) listed [here](https://www.learnopencv.com/install-opencv3-on-macos)

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

# Setting up Minotaur on Ubuntu 16.04
From a fresh install, you will need these packages

```bash
sudo apt install build-essential cmake python3 python3-dev qt5-default libudev-dev libopencv-dev
```

You will need to build OpenCV 3 from source. Download the OpenCV 3 source
and build. It is recommended to build with `USE_QT=ON` instead of GTK. 
Additional instructions found [here](https://github.com/BVLC/caffe/wiki/OpenCV-3.3-Installation-Guide-on-Ubuntu-16.04).

Then clone the repository and build with

```bash
git clone https://github.com/uwnrg/minotaur-cpp.git
cd minotaur-cpp
mkdir build
cd build
cmake ../
make
```

And then run with `./minotaur-cpp`

If Qt is unable to detect cameras, make sure to run

```bash
sudo apt install libqt5multimedia5-plugins
```

### Using the GOTURN pretrained model
The default tracker uses the MIL model, and may be configured to use other
tracker models. However, using the GOTURN model requires downloading and
adding to the working directory the architecture descriptor file `goturn.prototxt`
and the pretrained model data `goturn.caffemodel`. The latter is about 350 MB.
These files are hosted [here](https://github.com/Mogball/goturn-files). Once
acquired, add them to the working directory of the `minotaur-cpp` binary or
in the `CMakeLists.txt` directory.

### Building with Debug output off
Configure the CMake project with `cmake -DNO_DEBUG=ON ...`

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
