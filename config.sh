#!/bin/bash
PKG_CONFIG_PATH=/opt/ace-5.6.8/linux_amd64/lib/pkgconfig

export PKG_CONFIG_PATH

./configure $@
