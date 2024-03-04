use calamine::{open_workbook_auto, DeError, RangeDeserializerBuilder, Reader}; // Importación de las dependencias necesarias para leer archivos Excel.
use serde::Deserialize; // Importación del trait Deserialize para facilitar la deserialización de datos.
use std::cmp; // Importación para utilizar funciones de comparación.
use std::error::Error; // Importación de la trait Error para manejar errores.
use std::io::{self, ErrorKind}; // Importación para manejar entrada/salida y tipos de errores.

// Definición de una estructura que representa un proceso a partir de datos obtenidos de un archivo Excel.
#[derive(Debug, Deserialize)]
struct Proceso {
    #[serde(rename = "Nombre del Proceso")]
    nombre: String,
    #[serde(rename = "Tiempo de Llegada")]
    tiempo_llegada: u32,
    #[serde(rename = "Duración de la Ráfaga")]
    rafaga: u32,
}

// Definición de una estructura para representar un proceso con información adicional de tiempo.
#[derive(Debug)]
struct ProcesoConTiempo {
    nombre: String,
    tiempo_llegada: u32,
    rafaga: u32,
    tiempo_inicio: u32,
    tiempo_finalizacion: u32,
    tiempo_espera: u32,
}

// Función para ejecutar el algoritmo de planificación FCFS (First-Come, First-Served) en una lista de procesos.
fn fcfs(procesos: &mut [ProcesoConTiempo]) {
    let mut tiempo_actual: u32 = 0; // Inicialización del tiempo actual.

    // Iteración sobre cada proceso para calcular tiempos de inicio, finalización y espera.
    for proceso in procesos.iter_mut() {
        tiempo_actual = cmp::max(tiempo_actual, proceso.tiempo_llegada); // Se actualiza el tiempo actual si es necesario.
        proceso.tiempo_inicio = tiempo_actual; // Se asigna el tiempo de inicio del proceso.
        tiempo_actual += proceso.rafaga; // Se avanza el tiempo actual según la duración de la ráfaga del proceso.
        proceso.tiempo_finalizacion = tiempo_actual; // Se asigna el tiempo de finalización del proceso.
        proceso.tiempo_espera = proceso.tiempo_inicio - proceso.tiempo_llegada; // Se calcula el tiempo de espera del proceso.
    }
}

// Función para imprimir los resultados de la planificación de procesos.
fn imprimir_resultados(procesos: &[ProcesoConTiempo]) {
    // Impresión del encabezado de la tabla de resultados.
    println!(
        "{:<15} {:<15} {:<10} {:<15} {:<20} {:<15}",
        "Proceso",
        "Tiempo Llegada",
        "Ráfaga de CPU",
        "Tiempo Inicio",
        "Tiempo Finalización",
        "Tiempo Espera"
    );

    // Iteración sobre cada proceso para imprimir sus datos.
    for proceso in procesos {
        println!(
            "{:<15} {:<15} {:<10} {:<15} {:<20} {:<15}",
            proceso.nombre,
            proceso.tiempo_llegada,
            proceso.rafaga,
            proceso.tiempo_inicio,
            proceso.tiempo_finalizacion,
            proceso.tiempo_espera
        );
    }
}

// Función para leer datos de un archivo Excel y convertirlos en una lista de procesos.
fn leer_excel(ruta: &str) -> Result<Vec<ProcesoConTiempo>, Box<dyn Error>> {
    let mut workbook = open_workbook_auto(ruta)?; // Apertura del archivo Excel.

    // Obtención del rango de la hoja de cálculo 'Hoja1' o manejo de errores si no se encuentra.
    let range = workbook.worksheet_range("Hoja1").or_else(|e| {
        Err(io::Error::new(
            ErrorKind::NotFound,
            format!("No se encontró la hoja 'FCFS': {}", e),
        ))
    })?;

    let mut iter = RangeDeserializerBuilder::new().from_range(&range)?; // Creación de un iterador para deserializar los datos.

    let mut procesos = Vec::new(); // Inicialización de un vector para almacenar los procesos deserializados.
    while let Some(result) = iter.next() {
        // Iteración sobre los datos deserializados.
        let proceso: Result<Proceso, DeError> = result; // Intento de deserializar un proceso.
        match proceso {
            // Manejo de resultados exitosos o errores de deserialización.
            Ok(proceso) => {
                procesos.push(ProcesoConTiempo {
                    // Almacenamiento del proceso deserializado junto con datos de tiempo.
                    nombre: proceso.nombre,
                    tiempo_llegada: proceso.tiempo_llegada,
                    rafaga: proceso.rafaga,
                    tiempo_inicio: 0,
                    tiempo_finalizacion: 0,
                    tiempo_espera: 0,
                });
            }
            Err(e) => println!("Error al deserializar la fila: {}", e), // Manejo de errores de deserialización.
        }
    }

    Ok(procesos) // Devolución de la lista de procesos deserializados.
}

// Función principal que realiza la ejecución del programa.
fn main() -> Result<(), Box<dyn Error>> {
    let path: &str = "../../Planification Algorithms.xlsx"; // Ruta del archivo Excel.
    let mut procesos = leer_excel(path)?; // Lectura de datos del archivo Excel y manejo de errores.

    // Ejecutar el algoritmo de planificación FCFS y mostrar resultados.
    fcfs(&mut procesos);
    imprimir_resultados(&procesos);

    Ok(()) // Indicación de que el programa se ejecutó correctamente.
}
