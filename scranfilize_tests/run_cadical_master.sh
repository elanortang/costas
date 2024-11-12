#!/bin/bash

#TODO: Put in your own path variables to the binary executables
costas="/Users/estang/SAT/costas"
scranfilize="/Users/estang/SAT/scranfilize/scranfilize"
cadical="/Users/estang/SAT/cadical/build/cadical"

#TODO: I made a copy of this file for each variant, so that I could run them in parallel.
#(Editing the script while running it and then running it again seemed sketchy.) 
#TODO: choose your encoding and update 
encoding="col" #for file naming
output_dir="elanor"


for n in {4..20}
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
        sat_output="$costas/scranfilize_tests/${encoding}/n=${n}/sat/${filename}$i.sat"
        $cadical $scrambled_cnf > $sat_output
    done
done