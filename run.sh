#!/bin/bash

echo "Seleccione el algoritmo de planificación que desea simular:"
echo "1. FCFS (Rust)"
echo "2. SJF (Python)"
echo "3. Round Robin (C++)"
read -p "Ingrese su elección (1/2/3): " ALGORITMO


case "$ALGORITMO" in
  "1")
    # Suponiendo que el ejecutable de Rust se llama rust_fcfs
    cd fcsf/src
    cargo run
    ;;
  "2")
    # Suponiendo que el script de Python para SJF se llama python_sjf.py
    python3 python_sjf.py 
    ;;
  "3")
    # Suponiendo que el ejecutable de C++ para Round Robin se llama cpp_rr
    ./cpp_rr 
    ;;
  *)
    echo "Selección de algoritmo no válida."
    exit 1
    ;;
esac
