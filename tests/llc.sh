#!/bin/bash

echo "Running LLC-Olive in $1"
$OBJ_ROOT/bin/llc-olive $ROOT/tests/"$1".ll -o $ROOT/tests/"$1".s 2> $ROOT/tests/log.out
