#!/bin/bash

# TODO: Set paths to tool installations
scranfilize="/Users/estang/SAT/scranfilize/scranfilize"
cadical="/Users/estang/SAT/cadical/build/cadical"

# TODO: Set n to the desired number
n=18

# TODO: Choose your encoding
encoding="ord" 

# TODO: Set path to costas directory 
costas="/Users/estang/SAT/costas"

# TODO: Generate executable for generating the baseline cnf and set path to its location
encoding_program="$costas/${encoding}_variant/costas"

# TODO: Set path to directory for this n and this encording
path_to_n="$costas/scranfilize_tests/${encoding}/n=${n}"

# Layout:
# path_to_n
# |-- constraints (holds partial cnfs with symmetry constraints, generated by gen_sym.cpp)
# |-- sym_x0y0_top
# |    |-- cnf (holds scrambled cnfs)
# |    |-- timeout (holds SAT output from running cadical on scrambled cnfs)
# |    +-- {n}x{n}_{encoding}_scran0.cnf (File used to generate scrambled cnfs)
# |-- ... (same layout for all symmetry experiments)
# +-- num_successes.csv (tracks number of successful tries and if any experiments are unsat)

output_dir="timeout"
cnf_dir="cnf"
sym_cnfs="${path_to_n}/constraints"
successes_csv="${path_to_n}/num_successes.csv"

# Scranfilize options
options="-c 1 -v 0 -f 0 --force"

# Function for running each symmetry experiment
run () {
    x=$1
    y=$2
    kind=$3

    # Set path to this experiment
    path_to_output="${path_to_n}/sym_x${x}y${y}_${kind}"
    echo "${path_to_output}"

    # Make directories if they don't already exist
    if [ ! -d "${path_to_output}" ]; then
        mkdir -p "${path_to_output}"
    fi

    if [ ! -d "${path_to_output}/${output_dir}" ]; then
        mkdir -p "${path_to_output}/${output_dir}"
    fi

    if [ ! -d "${path_to_output}/${cnf_dir}" ]; then
        mkdir -p "${path_to_output}/${cnf_dir}"
    fi

    # Generate DIMACS file using your chosen encoding program
    filename="${n}x${n}_${encoding}_scran"
    original_cnf="${path_to_output}/${filename}0.cnf"
    $encoding_program $original_cnf $n

    # File with symmetry constraints
    sym_file="${sym_cnfs}/sym_n${n}_x${x}_y${y}_${kind}.cnf"
    num_lines=`wc -l < $sym_file`

    # Figure out the number of clauses for the updated header
    header=`tail -n 1 $original_cnf`
    echo $header
    num_clauses=${header##*[!0-9]}
    echo $num_clauses
    updated_num_clauses=$(( $num_clauses + $num_lines ))
    updated_header="${header%$num_clauses}$updated_num_clauses"
    echo $updated_header

    # Remove last line of cnf (the old header)
    file_size="$(stat -f=%z $original_cnf | cut -c 2- )"
    trim_count="$(tail -n1 "$original_cnf" | wc -c)"
    end_position="$(echo "$file_size - $trim_count" | bc)"
    dd of=`echo $original_cnf` if=/dev/null bs=1 seek="$end_position"

    # Write the updated header to the cnf
    echo "$updated_header" >> $original_cnf

    # Move header to the top
    vim $original_cnf -esc 'm0|wq' --not-a-term

    # Write symmetry constraints to the file
    cat $sym_file >> $original_cnf

    # Generate scranfilize tests and run cadical on them
    num_successes=0
    is_unsat=false
    for i in {1..100}
    do
        scrambled_cnf="${path_to_output}/${cnf_dir}/${filename}$i.cnf"
        $scranfilize $options $original_cnf $scrambled_cnf
        
        sat_output="${path_to_output}/${output_dir}/${filename}$i.sat"
        $cadical -t 10 $scrambled_cnf > $sat_output

        # If solution found, add "succ_" prefix to filenames and increment number of successes
        if [ "$(tail -n 1 $sat_output)" = "c exit 10" ]; then
            ((num_successes+=1))
            mv $scrambled_cnf "${path_to_output}/${cnf_dir}/succ_${filename}$i.cnf"
            mv $sat_output "${path_to_output}/${output_dir}/succ_${filename}$i.sat"
        
        # If cadical did not terminate, delete cnf and sat output files
        elif [ "$(tail -n 1 $sat_output)" = "c exit 0" ]; then
            rm $scrambled_cnf
            rm $sat_output
        
        # If cadical returned UNSAT, add "unsat_" prefix to filenames and break the loop. 
        # (Any scramble of these constraints will also be UNSAT, so no point in continuing.)
        elif [ "$(tail -n 1 $sat_output)" = "c exit 20" ]; then
            mv $scrambled_cnf "${path_to_output}/${cnf_dir}/unsat_${filename}$i.cnf"
            mv $sat_output "${path_to_output}/${output_dir}/unsat_${filename}$i.sat"
            is_unsat=true
            break
        fi
    done
    # The 0 means that this experiment was not unsat
    echo "${x}, ${y}, ${kind}, ${num_successes},${is_unsat}," >> $successes_csv
}

# BEGIN ACTUALLY RUNNING COMMANDS

# Make sure directories exist
if [ ! -d $path_to_n ]; then
    mkdir -p $path_to_n
fi

if [ ! -d $sym_cnfs ]; then
    mkdir -p $sym_cnfs
fi

rm $successes_csv
touch $successes_csv
echo "x, y, kind, num successes, unsat?," >> $successes_csv

# Generate symmetry DIMACS constraints
clang++ $costas/src/gen_sym.cpp -o gen_sym
./gen_sym $sym_cnfs $n

for x in {0,2,4}
do
    for y in {0,1,2}
    do
        run $x $y 'top'
    done
done

for x in {0,2,4}
do
    for y in {1,2}
    do
        if [[ $x -eq 0 && $y -eq 1 ]]; then
            continue
        fi
        run $x $y "edge"
    done
done