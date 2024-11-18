#!/bin/bash

#TODO: Put in your own path variables
costas="/Users/estang/SAT/costas"
scranfilize="/Users/estang/SAT/scranfilize/scranfilize"
cadical="/Users/estang/SAT/cadical/build/cadical"

#TODO: choose your encoding and update 
encoding_program="$costas/col_variant/costas"
encoding="ord" #for file naming

#TODO: choose the name of the cnf and output directories
output_dir="elanor_timeout"
cnf_dir="elanor_cnf"

#TODO: Double check scranfilize options
options="-c 1 -v 1 -f 0 --force"

#TODO: Set n to the desired number
n=21

echo "$costas/scranfilize_tests/${encoding}/n=${n}"
#make directory for the test if it doesn't already exist
if [ ! -d "$costas/scranfilize_tests/${encoding}/n=${n}" ]; then
    mkdir -p "$costas/scranfilize_tests/${encoding}/n=${n}"
fi

if [ ! -d "$costas/scranfilize_tests/${encoding}/n=${n}/${output_dir}" ]; then
    mkdir -p "$costas/scranfilize_tests/${encoding}/n=${n}/${output_dir}"
fi

if [ ! -d "$costas/scranfilize_tests/${encoding}/n=${n}/${cnf_dir}" ]; then
    mkdir -p "$costas/scranfilize_tests/${encoding}/n=${n}/${cnf_dir}"
fi

#generate dimacs file using your chosen encoding program
filename="${n}x${n}_${encoding}_scran"
original_cnf="$costas/scranfilize_tests/${encoding}/n=${n}/${filename}0.cnf"
$encoding_program $original_cnf $n

#moving header to the top
vim $original_cnf -esc 'm0|wq' --not-a-term

num_successes=0

#generate scranfilize tests
for i in {1..1000}
do
    # generate scranfilize tests
    scrambled_cnf="$costas/scranfilize_tests/${encoding}/n=${n}/${cnf_dir}/${filename}$i.cnf"
    $scranfilize $options $original_cnf $scrambled_cnf
    
    sat_output="$costas/scranfilize_tests/${encoding}/n=${n}/${output_dir}/${filename}$i.sat"
    $cadical -t 10 $scrambled_cnf > $sat_output

    if [ "$(tail -n 1 $sat_output)" = "c exit 10" ]; then
        ((num_successes+=1))
        mv $scrambled_cnf "$costas/scranfilize_tests/${encoding}/n=${n}/${cnf_dir}/succ_${filename}$i.cnf"
        mv $sat_output "$costas/scranfilize_tests/${encoding}/n=${n}/${output_dir}/succ_${filename}$i.sat"
        # echo $scrambled_cnf
        # echo $sat_output
    fi
done
echo $num_successes
