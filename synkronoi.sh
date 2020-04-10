#!/bin/bash
if [ $# -eq 0 ]
then
	echo "$0 : Usage: synkronoi <computer where to transfer>"
	echo "(Does not copy *.exe, *.o and *.Po files"
	echo ""
	exit 1
fi
# src-directory
echo "Excecuting command:"
echo "/usr/bin/rsync -trv --exclude="*.exe" --exclude="*.Po" --exclude="*.o" --exlude="lakritsi" --exclude="core.*" * $1:Projects/lakritsi/"
/usr/bin/rsync -trv --exclude="*.exe" --exclude="*.Po" --exclude="*.o" * $1:Projects/lakritsi/
# others
#echo "Moving other stuff"
#/usr/bin/rsync lakritsi.glade configure.in Makefile.am *.txt    -trv $1:Projects/lakritsi/

