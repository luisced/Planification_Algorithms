use calamine::{open_workbook_auto, DeError, RangeDeserializerBuilder, Reader};
use serde::Deserialize;
use std::cmp;
use std::error::Error;
use std::io::{self, ErrorKind};

#[derive(Debug, Deserialize)]
struct Proceso {
    #[serde(rename = "Nombre del Proceso")]
    nombre: String,
    #[serde(rename = "Tiempo de Llegada")]
    tiempo_llegada: u32,
    #[serde(rename = "Duración de la Ráfaga")]
    rafaga: u32,
}
#[derive(Debug)]
struct ProcesoConTiempo {
    nombre: String,
    tiempo_llegada: u32,
    rafaga: u32,
    tiempo_inicio: u32,
    tiempo_finalizacion: u32,
    tiempo_espera: u32,
}

fn fcfs(procesos: &mut [ProcesoConTiempo]) {
    let mut tiempo_actual: u32 = 0;

    for proceso in procesos.iter_mut() {
        tiempo_actual = cmp::max(tiempo_actual, proceso.tiempo_llegada);
        proceso.tiempo_inicio = tiempo_actual;
        tiempo_actual += proceso.rafaga;
        proceso.tiempo_finalizacion = tiempo_actual;
        proceso.tiempo_espera = proceso.tiempo_inicio - proceso.tiempo_llegada;
    }
}

fn imprimir_resultados(procesos: &[ProcesoConTiempo]) {
    println!(
        "{:<10} {:<15} {:<10} {:<15} {:<20} {:<15}",
        "Proceso",
        "Tiempo Llegada",
        "Ráfaga de CPU",
        "Tiempo Inicio",
        "Tiempo Finalización",
        "Tiempo Espera"
    );

    for proceso in procesos {
        println!(
            "{:<10} {:<15} {:<10} {:<15} {:<20} {:<15}",
            proceso.nombre,
            proceso.tiempo_llegada,
            proceso.rafaga,
            proceso.tiempo_inicio,
            proceso.tiempo_finalizacion,
            proceso.tiempo_espera
        );
    }
}

fn leer_excel(ruta: &str) -> Result<Vec<ProcesoConTiempo>, Box<dyn Error>> {
    let mut workbook = open_workbook_auto(ruta)?;
    let range = workbook.worksheet_range("Hoja1").or_else(|e| {
        Err(io::Error::new(
            ErrorKind::NotFound,
            format!("No se encontró la hoja 'FCFS': {}", e),
        ))
    })?;

    let mut iter = RangeDeserializerBuilder::new().from_range(&range)?;

    let mut procesos = Vec::new();
    while let Some(result) = iter.next() {
        let proceso: Result<Proceso, DeError> = result;
        match proceso {
            Ok(proceso) => {
                procesos.push(ProcesoConTiempo {
                    nombre: proceso.nombre,
                    tiempo_llegada: proceso.tiempo_llegada,
                    rafaga: proceso.rafaga,
                    tiempo_inicio: 0,
                    tiempo_finalizacion: 0,
                    tiempo_espera: 0,
                });
            }
            Err(e) => println!("Error al deserializar la fila: {}", e),
        }
    }

    Ok(procesos)
}

fn main() -> Result<(), Box<dyn Error>> {
    let path: &str = "../../Planification Algorithms.xlsx";
    let mut procesos = leer_excel(path)?;

    // Ejecutar FCFS y mostrar resultados
    fcfs(&mut procesos);
    imprimir_resultados(&procesos);

    Ok(())
}
