#!/bin/bash
./entry-generator 100000 | awk '{ cnt += length($0) } END { print cnt / NR }'