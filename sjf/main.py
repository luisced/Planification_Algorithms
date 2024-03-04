import pandas as pd #importamos la librería pandas como pd
ex = pd.read_excel('../Planification Algorithms.xlsx') #declaramos la variable ex para leer el archivo a traves de la ruta 

#declaramos diversas variables para así convertir cada columna en una lista
nombre_proceso = ex['Nombre del Proceso'].tolist() 
tiempos_llegada = ex['Tiempo de Llegada'].tolist()
rafagas = ex['Duración de la Ráfaga'].tolist()

procesos = list(zip(nombre_proceso, tiempos_llegada, rafagas)) #utilizamos una lista de tuplas para unir las listas anteriores
procesos.sort(key = lambda x: x[2]) #ordenamos cada proceso de acuerdo a su duración de ráfaga

for nombre_proceso in procesos:  #para cada proceso vamos a imprimir su informacion
    print(f"Proceso: {nombre_proceso[0]}, Tiempo de Llegada: {nombre_proceso[1]}, Ráfaga: {nombre_proceso[2]}")
