#!/bin/bash
#we do not support clang on osx..

#These are all hacks to get stuff not available on Ubuntu 12.xx
sudo add-apt-repository ppa:libreoffice/ppa -y
sudo add-apt-repository ppa:yade-users/external -y
sudo add-apt-repository ppa:ubuntu-toolchain-r/test -y 
sudo apt-get clean
sudo apt-get update -qq  
sudo apt-get update
if [ "$CXX" == "g++" ]; then 
	sudo apt-get install -qq g++-4.8; 
	export CXX="g++-4.8" CC="gcc-4.8"; 
         sudo update-alternatives --install /usr/bin/gcc gcc /usr/bin/gcc-4.8 40 --slave /usr/bin/g++ g++ /usr/bin/g++-4.8 
         sudo update-alternatives --config gcc 
fi
sudo apt-get install libann-dev libboost1.54-all-dev libtbb-dev libmetis-dev 
