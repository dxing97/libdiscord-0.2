#!/bin/bash
#supported distributions:
#Ubuntu 17.04 on x86_64
	 
sudo apt update && sudo apt upgrade
sudo apt install checkinstall libmicrohttpd-dev libjansson-dev libcurl4-gnutls-dev libgnutls28-dev libgcrypt20-dev git make cmake gcc libssl-dev libuv-dev libconfig-dev
cd ~/

#build from source and install ulfius and dependencies
git clone https://github.com/babelouest/ulfius.git
cd ulfius/
git submodule update --init
cd lib/orcania
make && sudo checkinstall
cd ../yder
make && sudo checkinstall
cd ../..
make
sudo checkinstall
cd ..

#build from source and install libwebsockets
git clone https://github.com/warmcat/libwebsockets
cd libwebsockets
mkdir build
cd build 
cmake ..
make
sudo checkinstall --pkgname libwebsockets
sudo ldconfig

#compile client 
#gcc -lwebsockets -lssl -lcrypto test-client.c -lwebsockets -lssl -lcrypto
