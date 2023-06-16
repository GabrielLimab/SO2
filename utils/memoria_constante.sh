#!/bin/bash

algorithms=("lru" "random" "2a" "fifo")
filenames=("compilador" "compressor" "matriz" "simulador")
sizes=("2" "4" "8" "16" "32" "64")

rm -rf ./memoria_constante
mkdir ./memoria_constante

for filename in "${filenames[@]}"
do
  for algorithm in "${algorithms[@]}"
  do
    for size in "${sizes[@]}"
    do
      output_file="${filename}_${algorithm}.txt"
      echo "Running ./main $algorithm $filename.log $size 1024"
      ./main $algorithm $filename.log $size 1024 >> ./memoria_constante/$output_file
      echo "Output saved in $output_file"
      echo ""
    done
  done
done
