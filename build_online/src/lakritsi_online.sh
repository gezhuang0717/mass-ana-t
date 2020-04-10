#!/bin/bash
export LD_LIBRARY_PATH=/opt/ace-5.6.8/linux_amd64/lib
./lakritsi -ORBDefaultInitRef 'iiop://corba-naming:4001'
