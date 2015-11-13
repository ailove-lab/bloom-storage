#!/bin/bash
for i in {0..200}
do
    echo $((i))
    redis-benchmark -q -t SET,GET -P $((i)) -n 100000 --csv
done

