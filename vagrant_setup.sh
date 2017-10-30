#!/usr/bin/env bash

# Installing Git
sudo apt-get update -qq
sudo apt-get install git -y -qq

# Installing Qt
wget http://download.qt.io/official_releases/qt/5.7/5.7.0/qt-opensource-linux-x64-5.7.0.run
chmod +x qt-opensource-linux-x64-5.7.0.run
sudo apt-get install build-essential libfontconfig1 mesa-common-dev libglu1-mesa-dev -y -qq

# Grab Qt installer script
git clone https://github.com/benlau/qtci.git
source qtci/path.env

# Extract Qt installation
extract-qt-installer qt-opensource-linux-x64-5.7.0.run /home/vagrant/Qt5.7.0
export PATH=/home/vagrant/Qt5.7.0/5.7/gcc_64/bin:$PATH

# Installing Python
sudo apt-get install python-software-properties -y -qq
sudo add-apt-repository ppa:fkrull/deadsnakes -y
sudo apt-get update -qq
sudo apt-get install python3.4 python3.4-dev -y -qq
python3.4 --version

# Install cmake
wget http://www.cmake.org/files/v3.2/cmake-3.2.2.tar.gz --no-check-certificate
tar xf cmake-3.2.2.tar.gz
cd cmake-3.2.2
./configure
make
sudo make install
cd ..
cmake --version

# Installing gcc
sudo add-apt-repository ppa:ubuntu-toolchain-r/test -y
sudo apt-get update -qq
sudo apt-get install gcc-4.8 g++-4.8 -y -qq
sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.8 20
sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-4.8 20
gcc --version
g++ --version

# Additional dependencies
sudo apt-get install libudev-dev libxi6 libsm6 libxrender1 libegl1-mesa -y -qq

# Build and run
cd ..
mkdir cmake-build-debug
cd cmake-build-debug
cmake ../minotaur-cpp
make
./minotaur-cpp