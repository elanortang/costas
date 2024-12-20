# Costas Arrays (Advanced Topics in Logic: Automated Reasoning and Satisfiability)

This repository contains code, data, and documentation for the Costas Array project

---

## Table of Contents
1. [Repository Structure]
2. [November 6-13 Experiments]
3. [November 12-20 Experiments]

## Repository Structure
This section explains the organization of the repository. Here's an overview of the directory layout:

```plaintext
costas/
├── docs/                            # Documents used in the project
│   ├── 24-solutions/                # Solutions for n=24
├── results/                         # Output of analyses and experiments
├── scripts/                         # Helper scripts for managing tasks
│   ├── cadical_timeout_ord.sh       # Script for running scranfilize timeout tests
│   ├── run_cadical_master.sh        # Script for running scranfilize runtime tests
│   └── make_scranfilize.sh          # Script for creating scrambled cnfs
├── src/                             # Source code files for the project
│   ├── base_gen_dimacs.cpp          # Outputs cnf using naive encoding
│   ├── bin_gen_dimacs.cpp           # Outputs cnf using binary encoding
│   ├── col_gen_dimacs.cpp           # Outputs cnf using column encoding
│   ├── ord_gen_dimacs.cpp           # Outputs cnf using order encoding
│   ├── isCostas.cpp                 # Checks if a solution is a satisfying solution
├── tests/  
│   ├── Nov6-13/                     # Scranfilize runtime tests
│   ├── Nov13-20/                    # Scranfilize timeout tests
└── README.md                        # This README file

## Nov6-13 Experiments
The goal was to understand the running times of the four encodings (which we've been calling base, binary, order, column) and ultimately understand on which encoding cadical solves the fastest. We created multiple cnf files, one for each encoding of the problems for n=4...20. We then created 10 scranfilized copies of each of those cnf files, multiplying the total number of files by 10. We then ran cadical (versions 1.0.3 and the latest version) on all of these files and recorded the runtimes corresponding to each cnf file. (Rose ran cadical version 1.0.3, and Elanor ran the latest cadical version.) The results of these experiments are recorded in the results folder (See "elanor_cadical.png" and "rose-cadical.png".) Based on these results, we have a slight reason to believe that the column encoding is the fastest.

## Nov13-20 Experiments

Note: After reorganization, file paths in scripts are probably broken. Hopefully no bugs were introduced when refactoring code - it was not run again after refactoring.