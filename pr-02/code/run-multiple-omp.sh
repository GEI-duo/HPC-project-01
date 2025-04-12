#!/bin/bash

prog=$1

case $prog in
    "serial" )
        program="heat_serial"
        ;;
    "omp" )
        program="heat_omp"
        ;;
    * )
        echo "Unknown program: $program. Options are 'serial' or 'omp'."
        exit 1
esac

threads=(1 2 4 8)
# steps=(100 1000)
steps=(100 1000 10000 100000)
# matrix=(100 1000)
matrix=(100 1000 2000)

for t in "${threads[@]}"; do
    for s in "${steps[@]}"; do
        for m in "${matrix[@]}"; do
            echo -e "===Running program '$program' with {threads=$t, steps=$s, matrix_size=$m}..."
            qsub -V -N "$prog"_"$t"_"$s"_"$m" -v OMP_NUM_THREADS="$t",OMP_PROGRAM="$program",STEPS="$s",SIZE="$m" run-simple-omp.sh
        done
    done
done

