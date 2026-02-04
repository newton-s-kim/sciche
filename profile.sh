#!/bin/bash

PROGRAMME=./build/sciche/sciche
GPROF=gprof
GPROF2DOT=gprof2dot

SCRIPT=$1
GMONOUT=${SCRIPT}.txt
DOTOUT=${SCRIPT}.png

. ~/mypy/bin/activate

$PROGRAMME $SCRIPT
$GPROF $PROGRAMME gmon.out > $GMONOUT
echo gmonout: $GMONOUT
cat $GMONOUT | $GPROF2DOT | dot -Tpng -o $DOTOUT
echo dotout: $DOTOUT

deactivate
