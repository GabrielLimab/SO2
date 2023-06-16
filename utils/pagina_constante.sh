#!/bin/bash

algorithms=("lru" "random" "2a" "fifo")
filenames=("compilador" "compressor" "matriz" "simulador")
sizes=("4")
buffers=("128" "256" "512" "1024" "2048" "4096" "8192" "16384")

rm -rf ./pagina_constante
mkdir ./pagina_constante

for filename in "${filenames[@]}"
do
  for algorithm in "${algorithms[@]}"
  do
    for buffer in "${buffers[@]}"
    do
      output_file="${filename}_${algorithm}.txt"
      echo "Running ./main $algorithm $filename.log 4 $buffer"
      ./main $algorithm $filename.log 4 $buffer >> ./pagina_constante/$output_file
      echo "Output saved in $output_file"
      echo ""
    done
  done
done
