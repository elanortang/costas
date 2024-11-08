#!/bin/bash

#TODO: Put in your own path variables
costas="/Users/rosesilv/Desktop/costas"
scranfilize="/Users/rosesilv/Desktop/scranfilize-master/scranfilize"

#TODO: choose your encoding and update 
encoding_program="$costas/col_variant/col_gen_dimacs"
encoding="col" #for file naming

#TODO: Double check scranfilize options
options="-c 1 -v 1 -f 0 --force"

for n in {4..20}
do
    echo "$costas/scranfilize_tests/${encoding}/n=${n}"
    #make directory for the test if it doesn't already exist
    if [ ! -d "$costas/scranfilize_tests/${encoding}/n=${n}" ]; then
        mkdir -p "$costas/scranfilize_tests/${encoding}/n=${n}"
    fi

    #generate dimacs file using your chosen encoding program
    filename="${n}x${n}_${encoding}_scran"
    original_cnf="$costas/scranfilize_tests/${encoding}/n=${n}/${filename}0.cnf"
    $encoding_program $original_cnf $n

    #moving header to the top
    vim $original_cnf -esc 'm0|wq' --not-a-term

    #generate scranfilize tests
    for i in {1..10}
    do
        scrambled_cnf="$costas/scranfilize_tests/${encoding}/n=${n}/${filename}$i.cnf"
        $scranfilize $options $original_cnf $scrambled_cnf
    done
done