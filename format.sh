#!/bin/bash

dir=$1

if [[ "$dir" == "" ]]; then
    dir="."
fi

find $dir -type f -name "*.cpp" -or -name "*.hpp" | while read -r line
do
    clang-format -i $line
done

