#!/bin/bash
for i in {0..1000}
do
    echo $((i*10))
    time redis-benchmark -q -t SET,GET -P $((i*10)) -n 1000000 --csv
done

