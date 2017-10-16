# Configuring Development Environment
This file will describe the necessary steps to setup a virtual development
environment on macOS Sierra 10.12.6 using Vagrant.

### Installing Vagrant
1. Download and install Vagrant from `https://www.vagrantup.com/downloads.html`
2. Download and install Oracle VirtualBox from `https://www.virtualbox.org/wiki/Downloads`

### Initializing Virtual Environment
1. Create a directory for Vagrant boxes `mkdir vagrant-boxes`
2. Setup the official box `vagrant init hashicorp/precise64`
3. Download and install XQuartz `https://www.xquartz.org/`
4. Edit the `Vagrantfile` created by `vagrant init` to have the line at the end

```config.ssh.forward_x11 = true```

5. Run `vagrant up`
6. Run `vagrant ssh-config`

### Configure the virtual environment
1. Access the box using `vagrant ssh`

#### Installing Qt
1. Download the Qt 5.7 installer `wget http://download.qt.io/official_releases/qt/5.7/5.7.0/qt-opensource-linux-x64-5.7.0.run`
2. Adjust permissions `chmod +x qt-opensource-linux-x64-5.7.0.run`
3. We must now install the required dependecies to run the installer

```
sudo apt-get update
sudo apt-get install build-essential
sudo apt-get install libfontconfig1
sudo apt-get install mesa-common-dev
sudo apt-get install libglu1-mesa-dev
```

4. Run the installer with `./qt-opensource-linux-x64-5.7.0.run`, and if all goes 
well, the installer should open in a new window through `XQuartz`
5. You can login, or click `Skip`
6. Hit `Deselect All` and select only `Desktop gcc 64-bit`
7. Accept and install; do not launch Qt Creator
8. After install add Qt to the path with `export PATH=/home/vagrant/Qt5.7.0/5.7/gcc_64/bin:/$PATH`

#### Installing Python
1. We require minimum Python 3.4, however Ubuntu 12.04 only goes up to Python 3.2
2. Run `sudo apt-get install python-software-properties` so that 
we have `add-apt-repository`
3. Add the PPA with Python 3.4 by running `sudo add-apt-repository ppa:fkrull/deadsnakes`
4. Run `sudo apt-get update`
5. Run `sudo apt-get install python3.4`
6. Run `sudo apt-get install python3.4-dev`

#### Install cmake
1. We require minimum cmake 3.1.0, so we will build cmake 3.2.2 from source
2. Download the source with `wget http://www.cmake.org/files/v3.2/cmake-3.2.2.tar.gz --no-check-certificate`
3. The `--no-check-certificate` is necessary because of Vagrant, and now install with

```
tar xf cmake-3.2.2.tar.gz
cd cmake-3.2.2
./configure
make
sudo make install
```

4. You can verify the installation with `cmake --version`

#### Cloning the repo
1. Install git with `sudo apt-get install git`
2. Clone the repo with `git clone https://github.com/mogball/minotaur-cpp.git`
3. Inside the repo, run `cmake CMakeLists.txt`
4. If all goes well, cmake will generate build files successfully

#### Installing gcc
1. We require gcc 4.8 to support C++11
2. Add the PPA `sudo add-apt-repository ppa:ubuntu-toolchain-r/test`
3. Install gcc 4.8 by running

```
sudo apt-get update
sudo apt-get install gcc-4.8 g++-4.8
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.8 20
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-4.8 20
```

4. You can verify that gcc is at version 4.8 by running `gcc --version`

#### Installing additional dependencies
1. We need to install a few more things before we can build and run minotaur-cpp

```
sudo apt-get install libudev-dev
sudo apt-get install libxi6
sudo apt-get install libsm6
sudo apt-get install libxrender1
sudo apt-get install libegl1-mesa
```

#### Running minotaur
1. You can now run minotaur with `./minotaur-cpp`
