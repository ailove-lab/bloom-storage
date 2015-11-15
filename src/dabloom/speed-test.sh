#!/bin/bash
#write ~850 MB
dd if=/dev/zero of=./test bs=8k count=100k
