#!/bin/sh

path=$1

if [ ! -d ${path} ]; then
    # path exit
    mkdir ${path}
fi