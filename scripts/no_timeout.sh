#!/bin/bash

#TODO: Put in your own path variables to the binary executables
costas="/Users/estang/SAT/costas"
scranfilize="/Users/estang/SAT/scranfilize/scranfilize"
cadical="/Users/estang/SAT/cadical/build/cadical"

#TODO: choose your encoding and update 
# encoding_program="$costas/col_variant/col_gen_dimacs"
encoding="col" #for file naming

#TODO: Double check scranfilize options
options="-c 1 -v 0 -f 0 --force"

for n in {4..20}
do
    echo "$costas/scranfilize_tests/${encoding}/n=${n}"
    # make directory for the test if it doesn't already exist
    if [ ! -d "$costas/scranfilize_tests/${encoding}/n=${n}/sat" ]; then
        mkdir -p "$costas/scranfilize_tests/${encoding}/n=${n}/sat"
    fi

    #generate dimacs file using your chosen encoding program
    filename="${n}x${n}_${encoding}_scran"
    # original_cnf="$costas/scranfilize_tests/${encoding}/n=${n}/${filename}0.cnf"
    # $encoding_program $original_cnf $n

    #moving header to the top
    # vim $original_cnf -esc 'm0|wq' --not-a-term

    #run cadical on scranfilize tests
    for i in {1..10}
    do
        scrambled_cnf="$costas/scranfilize_tests/${encoding}/n=${n}/${filename}$i.cnf"
        sat_output="$costas/scranfilize_tests/${encoding}/n=${n}/sat/${filename}$i.sat"
        # $scranfilize $options $original_cnf $scrambled_cnf
        $cadical $scrambled_cnf > $sat_output
    done
done