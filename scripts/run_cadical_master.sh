#!/bin/bash

#TODO: Put in your own path variables to the binary executables
costas="/Users/rosesilv/Desktop/costas"
scranfilize="/Users/rosesilv/Desktop/scranfilize-master/scranfilize"
cadical="/Users/rosesilv/Desktop/cadical-1.0.3-cb89cbf/build/cadical"

#TODO: I made a copy of this file for each variant, so that I could run them in parallel.
#(Editing the script while running it and then running it again seemed sketchy.) 
#TODO: choose your encoding and update 
encoding="bin" #for file naming
output_dir="rose"
data_file="/Users/rosesilv/Desktop/data-cadical1.0.3/bin-data.csv"

for n in {4..19}
do
    echo "$costas/scranfilize_tests/${encoding}/n=${n}"
    # make directory for the test if it doesn't already exist
    if [ ! -d "$costas/scranfilize_tests/${encoding}/n=${n}/${output_dir}" ]; then
        mkdir -p "$costas/scranfilize_tests/${encoding}/n=${n}/${output_dir}"
    fi

    #name of dimacs file using your chosen encoding program
    filename="${n}x${n}_${encoding}_scran"

    #run cadical on scranfilize tests
    for i in {1..10}
    do
        scrambled_cnf="$costas/scranfilize_tests/${encoding}/n=${n}/${filename}$i.cnf"
        sat_output="$costas/scranfilize_tests/${encoding}/n=${n}/${output_dir}/${filename}$i.sat"

        start=$(date +%s)
        $cadical $scrambled_cnf > $sat_output
        end=$(date +%s)
        # Calculate elapsed time in milliseconds
        elapsed_time=$((end - start))
        echo "$n,$i,$elapsed_time" >> "$data_file"
    done
done