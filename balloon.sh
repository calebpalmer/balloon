#!/bin/bash

curdir=$(pwd)
export PATH=$curdir/capengine:$PATH
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$curdir/capengine:.

#gdb -batch -ex "run" -ex "bt" -e "balloon" 2>&1 | grep -v ^"No stack."$
gdb -batch -ex "run $@" -ex "bt" -e "balloon" 2>&1 | grep -v ^"No stack."$
