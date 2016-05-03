#!/bin/bash
#we do not support clang on ubuntu 12.x..
CC=gcc
CXX=g++
#These are all hacks to get stuff not available on Ubuntu 12.xx
sudo add-apt-repository ppa:libreoffice/ppa -y
sudo add-apt-repository ppa:yade-users/external -y
#sudo add-apt-repository ppa:ubuntu-toolchain-r/test -y 
sudo apt-get clean
sudo apt-get update -qq  
sudo apt-get install libann-dev libboost1.54-all-dev libtbb-dev libmetis-dev 
