#!/bin/bash

#TODO: Put in your own path variables
costas="/Users/estang/SAT/costas"
scranfilize="/Users/estang/SAT/scranfilize/scranfilize"
cadical="/Users/estang/SAT/cadical/build/cadical"

n=24

#TODO: choose your encoding and update 
encoding_program="$costas/ord_variant/costas"
encoding="col" #for file naming
path_to_output="$costas/scranfilize_tests/${encoding}/n=${n}/freq2"

#TODO: choose the name of the cnf and output directories
output_dir="elanor_timeout"
cnf_dir="elanor_cnf"

#TODO: Double check scranfilize options
options="-c 1 -v 0 -f 0 --force"

#TODO: Set n to the desired number

echo "${path_to_output}"
#make directory for the test if it doesn't already exist
if [ ! -d "$costas/scranfilize_tests/${encoding}/n=${n} "]; then
    mkdir -p "$costas/scranfilize_tests/${encoding}/n=${n}"
fi

if [ ! -d "${path_to_output}" ]; then
    mkdir -p "${path_to_output}"
fi

if [ ! -d "${path_to_output}/${output_dir}" ]; then
    mkdir -p "${path_to_output}/${output_dir}"
fi

if [ ! -d "${path_to_output}/${cnf_dir}" ]; then
    mkdir -p "${path_to_output}/${cnf_dir}"
fi

#generate dimacs file using your chosen encoding program
filename="${n}x${n}_${encoding}_scran"
original_cnf="${path_to_output}/${filename}0.cnf"
# $encoding_program $original_cnf $n

#moving header to the top
# vim $original_cnf -esc 'm0|wq' --not-a-term

num_successes=0

#generate scranfilize tests
for i in {1..1000}
do
    # generate scranfilize tests
    scrambled_cnf="${path_to_output}/${cnf_dir}/${filename}$i.cnf"
    $scranfilize $options $original_cnf $scrambled_cnf
    
    sat_output="${path_to_output}/${output_dir}/${filename}$i.sat"
    $cadical -t 10 $scrambled_cnf > $sat_output

    if [ "$(tail -n 1 $sat_output)" = "c exit 10" ]; then
        ((num_successes+=1))
        mv $scrambled_cnf "${path_to_output}/${cnf_dir}/succ_${filename}$i.cnf"
        mv $sat_output "${path_to_output}/${output_dir}/succ_${filename}$i.sat"
        # echo $scrambled_cnf
        # echo $sat_output
    elif [ "$(tail -n 1 $sat_output)" = "c exit 0" ]; then
        rm $scrambled_cnf
        rm $sat_output
    fi
done
echo $num_successes
