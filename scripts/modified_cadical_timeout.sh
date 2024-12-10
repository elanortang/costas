#!/bin/bash

#TODO: Put in your own path variables
costas="/Users/rosesilver/Desktop/costas"
scranfilize="/Users/rosesilver/Desktop/scranfilize-master/scranfilize"
cadical="/Users/rosesilver/Desktop/automated-reasoning-f24/cadical/build/cadical"

#TODO: Double check scranfilize options
options="-c 1 -v 1 -f 0 --force"

#TODO: Set n to the desired number
n=24

#TODO: generate dimacs file using your chosen encoding program
filename="${n}_col_scran"
original_cnf="$costas/tests/Nov13-20/cnf/24_col.cnf"

num_successes=0

echo "Starting scranfilize tests"

for i in {1..1000}
do
    # generate scranfilize tests
    scrambled_cnf="$costas/tests/Nov13-20/cnf/${filename}$i.cnf"
    echo "$scrambled_cnf"
    $scranfilize $options $original_cnf $scrambled_cnf
  
    sat_output="$costas/tests/Nov13-20/sat/${filename}$i.sat"
    $cadical -t 10 $scrambled_cnf > $sat_output

    if [ "$(tail -n 1 $sat_output)" = "c exit 10" ]; then
        ((num_successes+=1))
    else 
        rm "$sat_output"
        rm "$scrambled_cnf"
    fi
done
echo $num_successes
