#!/bin/bash
set +e
set -x

cd build
make test
if [ $? != 0 ] || [ ! -f "fp_exec" ] ; then
    exit 1
fi
