#!/bin/bash
LibDir=$PWD
Target=Sunny
lib_array=($(ldd $Target | grep -o "/.*" | grep -o "/.*/[^[:space:]]*"))
$(mkdir $LibDir)
for Variable in ${lib_array[@]}
do
    cp "$Variable" $LibDir
done