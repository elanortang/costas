#!/bin/bash

# TODO: Set paths to tool installations
scranfilize="/Users/estang/SAT/scranfilize/scranfilize"
cadical="/Users/estang/SAT/cadical/build/cadical"

# TODO: Set path to costas directory 
costas="/Users/estang/SAT/costas"

sym_constraints="${costas}/sym_constraints"
cnfs="${costas}/sym_cnfs"

# Function for running each symmetry experiment
run () {
    x=$1
    y=$2
    kind=$3
    encoding=$4

    # TODO: Generate executable for generating the baseline cnf and set path to its location
    encoding_program="$costas/${encoding}_variant/costas"

    # Generate DIMACS file using your chosen encoding program
    original_cnf="${cnfs}/n${n}_${encoding}_x${x}y${y}_${kind}.cnf"
    $encoding_program $original_cnf $n

    # File with symmetry constraints
    sym_file="${sym_constraints}/sym_n${n}_x${x}y${y}_${kind}.cnf"
    num_lines=`wc -l < $sym_file`

    # Figure out the number of clauses for the updated header
    header=`tail -n 1 $original_cnf`
    num_clauses=${header##*[!0-9]}
    updated_num_clauses=$(( $num_clauses + $num_lines ))
    updated_header="${header%$num_clauses}$updated_num_clauses"

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
}

# BEGIN ACTUALLY RUNNING COMMANDS

# Make sure directories exist
if [ ! -d $sym_constraints ]; then
    mkdir -p $sym_constraints
fi

if [ ! -d $cnfs ]; then
    mkdir -p $cnfs
fi


for ((n=18; n <= 32; n+=2))
do
    # Generate symmetry DIMACS constraints
    clang++ $costas/src/gen_sym.cpp -o gen_sym
    ./gen_sym $sym_constraints $n

    for encoding in "base" "bin" "col" "ord"
    do
        for x in {0,2,4}
        do
            for y in {0,1,2}
            do
                run $x $y 'top' $encoding
            done
        done

        for x in {0,2,4}
        do
            for y in {1,2}
            do
                if [[ $x -eq 0 && $y -eq 1 ]]; then
                    continue
                fi
                run $x $y "edge" $encoding
            done
        done
    done
done