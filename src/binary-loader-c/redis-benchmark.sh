#!/bin/bash
for i in {1..100}
do
    echo $((i*100))
    time redis-benchmark -q -t SET,GET -P $((i*100)) -n 1000000 --csv
done

