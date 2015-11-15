#!/bin/bash
file=../../data/data-sample-keys
printf "\n--- create b\n"
echo create b | nc localhost 8673
du -sh $file
printf "\n--- fill\n"
time cat $file | paste -s -d ' ' | sed -e 's/^/b b /' | nc localhost 8673 >/dev/null
printf "\n--- list\n"
echo list | nc localhost 8673
printf "\n--- info b\n"
echo info b | nc localhost 8673
printf "\n--- flush \n"
echo flush b | nc localhost 8673
printf "\n--- info b\n"
echo info b | nc localhost 8673