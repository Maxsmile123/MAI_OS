#!/bin/bash

# $1 = m(Use medium value),

gcc lab3.c queue.h queue.c -lm -pthread
echo "hello" > dataset.txt

for ((i = 1; i < 60; i++)) # i = number of threads
do
    count=0.0
    if [ -n "$1" ]
    then
        if [ $1 = "m" ]
        then
            for ((j = 0; j < 10; j++))
            do
                res=$(./a.out -t $i)
                count=$(bc<<<"scale=7;$count + $res")
            done
            count=$(bc<<<"scale=7;$count / 10")
        fi
    else
        res=$(./a.out -t $i)
        count=$(bc<<<"scale=7;$count + $res")
    fi
    if (( $(echo "$count < 1.0" |bc -l) )); then
        echo -n "0" >> dataset.txt
    fi
    echo -n "$count" >> dataset.txt
    echo -n " " >> dataset.txt
done