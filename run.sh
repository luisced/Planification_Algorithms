#!/bin/bash

while true; do
    clear
    cat << "EOF"

           _                  _ _                           
     /\   | |                (_) |                          
    /  \  | | __ _  ___  _ __ _| |_ _ __ ___   ___  ___     
   / /\ \ | |/ _` |/ _ \| '__| | __| '_ ` _ \ / _ \/ __|    
  / ____ \| | (_| | (_) | |  | | |_| | | | | | (_) \__ \    
 /_/    \_\_|\__, |\___/|_|  |_|\__|_| |_| |_|\___/|___/    
 |  __ \      __/ |                                         
 | |  | | ___|___/                                          
 | |  | |/ _ \                                              
 | |__| |  __/                                              
 |_____/ \___|         _  __ _                _             
 |  __ \| |           (_)/ _(_)              (_)            
 | |__) | | __ _ _ __  _| |_ _  ___ __ _  ___ _  ___  _ __  
 |  ___/| |/ _` | '_ \| |  _| |/ __/ _` |/ __| |/ _ \| '_ \ 
 | |    | | (_| | | | | | | | | (_| (_| | (__| | (_) | | | |
 |_|    |_|\__,_|_| |_|_|_| |_|\___\__,_|\___|_|\___/|_| |_|

+----+-------------+----------------------+
| #  |  Algorithm  | Programming Language |
+----+-------------+----------------------+
| 1º | FCSF        | Rust                 |
| 2º | SJF         | Python               |
| 3º | Round Robin | C++                  |
+----+-------------+----------------------+

EOF

    read -p "Seleccione el algoritmo de planificación que desea simular (1-3) o '4' para salir: " ALGORITMO

    case "$ALGORITMO" in
      "1")
        # Suponiendo que el ejecutable de Rust se llama rust_fcfs
        (cd fcsf/src && cargo run)
        ;;
      "2")
        # Suponiendo que el script de Python para SJF se llama python_sjf.py
        python3 python_sjf.py
        ;;
      "3")
        # Suponiendo que el ejecutable de C++ para Round Robin se llama cpp_rr
        ./cpp_rr
        ;;
      "4")
        echo "Saliendo del programa."
        exit 0
        ;;
      *)
        echo "Selección de algoritmo no válida. Intente de nuevo."
        ;;
    esac
    read -p "Presione Enter para continuar..."
done
