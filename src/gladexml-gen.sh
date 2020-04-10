#!/bin/sh

PROG="python ../util/glade-helper.py"

generate_includes()
{
    ARGS=""
    w=$1
    h=$2
    shift 2
    if [ -n ${1:-} ]; then
	for foo in $*; do
	    ARGS="$ARGS --${foo}"
	done
    fi

    echo "Generating stuff for $h"
	
    ${PROG} $ARGS --headers ../lakritsi.glade "${w}" "${h}" > "${w}"_headers.inc
    ${PROG} $ARGS --members ../lakritsi.glade "${w}" "${h}" > "${w}"_members.inc
    ${PROG} $ARGS --init ../lakritsi.glade "${w}" "${h}" > "${w}"_init.inc
}

generate_includes window1 Window1 subclassed=ramseyer:Ramseyer,fit_parameters:Fit_Parameters,inputting:Inputting,tickerer:Tickerer
#generate_includes window2 Window2
generate_includes postProcessor Post_Processor
generate_includes ramseyer Ramseyer subclassed=fit_parameters:Fit_Parameters
generate_includes tickerer Tickerer
generate_includes fit_parameters Fit_Parameters
generate_includes inputting Inputting

generate_includes dialog1 Dialog1
generate_includes ok_cancel_dialog OK_Cancel_Dialog
generate_includes ok_cancel_dialog_2 ok_cancel_dialog_2
