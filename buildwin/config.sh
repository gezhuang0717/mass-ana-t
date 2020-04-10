#!/bin/sh
# cross.env
#Töissä:
PREFIX=/home/tomero/mingw_kaanto
CORBA_PREFIX=/opt/ace-5.6.8/win32
#Kotona:
#PREFIX=/home/tomero/alas/
TARGET=mingw32
export CC="i586-mingw32msvc-gcc"
export CXX="i586-mingw32msvc-g++"
export CFLAGS="-O -march=i586"
export CXXFLAGS="-O -march=i586"
export PKG_CONFIG_PATH=$PREFIX/GTK/lib/pkgconfig:$PREFIX/curl/lib/pkgconfig:$CORBA_PREFIX/lib/pkgconfig
export LDFLAGS=-L$PREFIX/lib

../configure --host i586-mingw32msvc --enable-windows $@
