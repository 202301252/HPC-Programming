#!/bin/bash

echo "Running optimized interpolation benchmark"
echo "-----------------------------------------"

cases=("250 100 900000 10"
       "250 100 5000000 10"
       "500 200 3600000 10"
       "500 200 20000000 10"
       "1000 400 14000000 10")

labels=("a" "b" "c" "d" "e")

for i in ${!cases[@]}
do
    params=${cases[$i]}
    label=${labels[$i]}

    echo "Case $label : $params"

    # generate input.bin
    echo $params | ./input_maker.out > /dev/null

    # run and capture elapsed time
    time_output=$(/usr/bin/time -f "%e" ./main.out input.bin 2>&1)

    echo "Time = $time_output sec"
    echo ""
done
