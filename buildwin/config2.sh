#!/bin/sh

CURL_PREFIX=/home/tomero/Projects/libcurl_win
CORBA_PREFIX=/opt/ace-5.6.8/win32
#Kotona:
#PREFIX=/home/tomero/alas/
TARGET=mingw32
export CC="i686-w64-mingw32-gcc"
export CXX="i686-w64-mingw32-g++"
export AR="i686-w64-mingw32-ar"
export CFLAGS="-O -march=i686"
export CXXFLAGS="-O -march=i686"
export RANLIB="i686-w64-mingw32-ranlib"
#export PKG_CONFIG_PATH=/home/tomero/Projects/gtkmm_win/lib/pkgconfig:$CURL_PREFIX/lib/pkgconfig:$CORBA_PREFIX/lib/pkgconfig
export PKG_CONFIG_PATH=/home/tomero/Projects/gtkmm_win/lib/pkgconfig:$CURL_PREFIX/lib/pkgconfig:$CORBA_PREFIX/lib/pkgconfig
export LDFLAGS=-L/cross_compile/gtkmm/lib

../configure --host i686-w64-mingw32 --enable-windows $@
