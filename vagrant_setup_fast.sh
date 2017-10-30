#!/usr/bin/env bash

sudo apt-get update -qq
sudo apt-get install python-software-properties -y -qq

sudo add-apt-repository ppa:fkrull/deadsnakes -y
sudo add-apt-repository ppa:george-edison55/precise-backports -y
sudo add-apt-repository ppa:ubuntu-toolchain-r/test -y
sudo add-apt-repository ppa:canonical-qt5-edgers/ubuntu1204-qt5 -y

sudo apt-get update -qq

sudo apt-get install qtdeclarative5-dev -y -qq
sudo apt-get install python3.4 python3.4-dev -y -qq
sudo apt-get install cmake -y -qq
sudo apt-get install g++-4.8 -y -qq

sudo update-alternatives --install /usr/bin/g++ g++ /usr/bin/g++-4.8 20

python3.4 --version
cmake --version
g++ --version

sudo apt-get install libudev-dev libxi6 libsm6 libxrender1 libegl1-mesa -y -qq

cd ..
mkdir cmake-build-debug
cd cmake-build-debug
cmake ../minotaur-cpp
make
./minotaur-cpp
