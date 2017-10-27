#!/bin/bash
#dependency installer for libdiscord bot2
#eventually it'll also install libdiscord itself as well as any bots but for now it just takes care of dependencies
#many improvements can be made to this script

##assumptions:
#You're using a debian based (apt) linux distribution

#supported distributions:
#Ubuntu 17.04 on x86_64
#todo: test Debian(Raspbian) on Raspberry Pi, CentOS on x86, SunOS on SPARC (especially CentOS and SunOS. Can't apt your way through those...)
#try using uname as well, especially for SunOS/Solaris detection (returns "Linux" for other kernels)
#windows is probably not going to be supported anytime soon
#todo: add option to upgrade to the latest version of libDiscord (update dependencies and stuff)
##Default versions of Ulfius and libwebsockets for this version of libDiscord
LWS_VER=2.4.0
ULFIUS_VER=2.1.2

#todo: use uname, as lsb_release isn't portable across all *nixes.
OS=$(lsb_release -si)
VER=$(lsb_release -sr)

case ${OS} in
    "Ubuntu")
        #echo Detected Ubuntu
        case ${VER} in
            "17.04")
                echo Detected supported $OS version \($VER\)
            ;;
            *)
                echo This version of ${OS} \(${VER}\) is not supported. Installation might not work.
                read -rsp $'Press any key to contunue\n' -n1 key
            ;;
        esac
    ;;
    "Debian")
        case ${VER} in
            "9.0.0" | "9.0.1")
                echo Detected supported ${OS} version \(${VER}\)
            ;;
            *)
                echo This version of ${OS} \(${VER}\) is not supported. Installation might not work.
                read -rsp $'Press any key to continue\n' -n1 key
            ;;
#    "Elementary OS")
#        echo Detected Elementary OS
#        case $VER in
#            "1.0")
#                echo Detected supported $OS version \($VER\)
        esac
esac

#is apt even installed? detect package manager.
sudo apt update && sudo apt upgrade

#todo: detect the version of different dependencies for extra assurance that they work. Warn otherwise.
#maybe some combos of dependencies work but some don't? try checking over a wide range of package versions
#todo: add yum stuff for CentOS/RHEL distros
#todo: if a package can't be found (or packages can't be installed in general, maybe ask if we can compile dependencies from source?
#especially if the user doesn't have superuser privileges or packages just aren't available for that specific distro.

sudo apt install checkinstall libmicrohttpd-dev libjansson-dev libcurl4-gnutls-dev libgnutls28-dev libgcrypt20-dev git make cmake gcc libssl-dev libuv-dev libconfig-dev
cd ~/


#todo: detect if ulfius and libwebsockets are already installed (really anything that needs to be compiled from source)
#if they're already installed, check their version (if there is one)

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
git checkout v${LWS_VER}
mkdir build
cd build 
cmake ..
make
sudo checkinstall --pkgname libwebsockets
sudo ldconfig

#todo: add bot2 compilation and installation here as well
