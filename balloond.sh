#!/bin/bash

curdir=$(pwd)
export PATH=$curdir/capengine:$PATH
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:$curdir/capengine:.

gdb -ex "dir capengine" balloon

