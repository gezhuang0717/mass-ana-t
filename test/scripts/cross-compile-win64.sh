#!/bin/sh

set -e

GTKMM_PREFIX=/opt/gtkmm64
ACE_PREFIX=/opt/ace-6.2.6/win64

TARGET=x86_64-w64-mingw32
export CC="$TARGET-gcc"
export CXX="$TARGET-g++"
export CFLAGS="-O -g"
export CXXFLAGS="-O -g"
export PKG_CONFIG_PATH=${GTKMM_PREFIX}/lib/pkgconfig:${ACE_PREFIX}/lib/pkgconfig
export LDFLAGS="-L${GTKMM_PREFIX}/lib -fno-lto"

if [ ! -f lakritsi.xml ] || [ ! -d .git ]; then
  echo "This script needs to be run on top directory."
  exit 1
fi

TEMPDIR=`mktemp -d --suffix=-lakritsi-build`
NUMJOBS=6

# export CXX=clang++

if test x$KEEP = x; then
trap "{ rm -fr $TEMPDIR ; exit 255; }" EXIT
fi

git archive --format=tar HEAD . | (cd $TEMPDIR ; tar xfp -)
git describe --always > ${TEMPDIR}/src/VERSION

pushd ${TEMPDIR}
test -f lakritsi.xml
echo "Copy of files succesful."

autoreconf -fi
if [ -f src/gtkbuilder-gen.sh ]; then
  pushd src
  sh gtkbuilder-gen.sh
  popd
fi

mkdir -p build/gtk2
pushd build/gtk2
../../configure --host $TARGET --enable-windows \
--with-boost=${GTKMM_PREFIX} --with-gtkmm-version=gtkmm2
echo "Configuration of gtkmm2 version successful."
make -j${NUMJOBS}
echo "Build of gtkmm2 version succesful."
popd

mkdir -p build/gtk3
pushd build/gtk3
../../configure --host $TARGET --enable-windows \
--with-boost=${GTKMM_PREFIX} --with-gtkmm-version=gtkmm3
echo "Configuration of gtkmm3 successful."
make -j${NUMJOBS}
echo "Build of gtkmm3 version succesful."
popd

mkdir -p build/gtk2-online
pushd build/gtk2-online
../../configure --host $TARGET --enable-windows \
--with-boost=${GTKMM_PREFIX} --with-gtkmm-version=gtkmm2 --with-online
echo "Configuration of gtkmm2 / online successful."
make -j${NUMJOBS}
echo "Build of gtkmm2 online version succesful."
popd

mkdir -p build/gtk3-online
pushd build/gtk3-online
../../configure --host $TARGET --enable-windows \
--with-boost=${GTKMM_PREFIX} --with-gtkmm-version=gtkmm3 --with-online
echo "Configuration of gtkmm3 / online successful."
make -j${NUMJOBS}
echo "Build of gtkmm3 online version succesful."
popd

echo "All builds succesful."

if test x$KEEP = x; then
  rm -rf *
else
  echo "Kept the sources in ${TEMPDIR}"
fi
popd
