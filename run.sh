use calamine::{open_workbook_auto, Reader, RangeDeserializerBuilder, DeError};
use serde::Deserialize;
use std::cmp;
use std::error::Error;

#[derive(Debug, Deserialize)]
struct Proceso {
    nombre: String,
    tiempo_llegada: f64, // En Excel los números se manejan como f64
    rafaga: f64,
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
    let mut tiempo_actual = 0;

    for proceso in procesos.iter_mut() {
        tiempo_actual = cmp::max(tiempo_actual, proceso.tiempo_llegada);
        proceso.tiempo_inicio = tiempo_actual;
        tiempo_actual += proceso.rafaga;
        proceso.tiempo_finalizacion = tiempo_actual;
        proceso.tiempo_espera = proceso.tiempo_inicio - proceso.tiempo_llegada;
    }
}

fn imprimir_resultados(procesos: &[ProcesoConTiempo]) {
    println!("{:<10} {:<15} {:<10} {:<15} {:<20} {:<15}", "Proceso", "Tiempo Llegada", "Ráfaga de CPU", "Tiempo Inicio", "Tiempo Finalización", "Tiempo Espera");

    for proceso in procesos {
        println!("{:<10} {:<15} {:<10} {:<15} {:<20} {:<15}",
            proceso.nombre,
            proceso.tiempo_llegada,
            proceso.rafaga,
            proceso.tiempo_inicio,
            proceso.tiempo_finalizacion,
            proceso.tiempo_espera);
    }
}

fn leer_excel(ruta: &str) -> Result<Vec<ProcesoConTiempo>, Box<dyn Error>> {
    let mut workbook = open_workbook_auto(ruta)?;
    let range = workbook
        .worksheet_range("FCFS")
        .ok_or_else(|| calamine::Error::Msg("No se encontró la hoja 'FCFS'"))??;

    let mut iter = RangeDeserializerBuilder::new().from_range(&range)?;

    let mut procesos = Vec::new();
    for row in iter {
        let proceso: Result<Proceso, DeError> = row;
        match proceso {
            Ok(proceso) => {
                procesos.push(ProcesoConTiempo {
                    nombre: proceso.nombre,
                    tiempo_llegada: proceso.tiempo_llegada as u32,
                    rafaga: proceso.rafaga as u32,
                    tiempo_inicio: 0,
                    tiempo_finalizacion: 0,
                    tiempo_espera: 0,
                });
            },
            Err(e) => println!("Error al deserializar la fila: {}", e),
        }
    }

    Ok(procesos)
}

fn main() -> Result<(), Box<dyn Error>> {
    let path = "ruta/a/tu/archivo/Planificacion Algoritmos.xlsx";
    let mut procesos = leer_excel(path)?;

    // Ejecutar FCFS y mostrar resultados
    fcfs(&mut procesos);
    imprimir_resultados(&procesos);

    Ok(())
}
