
use std::cmp;

#[derive(Debug)]
struct Proceso {
    nombre: String,
    tiempo_llegada: u32,
    rafaga: u32,
    tiempo_inicio: u32,
    tiempo_finalizacion: u32,
    tiempo_espera: u32,
}

fn fcfs(procesos: &mut Vec<Proceso>) {
    let mut tiempo_actual = 0;

    for proceso in procesos.iter_mut() {
        tiempo_actual = cmp::max(tiempo_actual, proceso.tiempo_llegada);
        proceso.tiempo_inicio = tiempo_actual;
        tiempo_actual += proceso.rafaga;
        proceso.tiempo_finalizacion = tiempo_actual;
        proceso.tiempo_espera = proceso.tiempo_inicio - proceso.tiempo_llegada;
    }
}

fn imprimir_resultados(procesos: &Vec<Proceso>) {
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

fn main() {
    let mut procesos: Vec<Proceso> = Vec::new();
    
    // Aquí iría el código para la entrada del usuario, pero para propósitos de demostración, vamos a usar datos estáticos.
    procesos.push(Proceso {
        nombre: "Outlook".to_string(),
        tiempo_llegada: 0,
        rafaga: 5,
        tiempo_inicio: 0,
        tiempo_finalizacion: 0,
        tiempo_espera: 0,
    });
    procesos.push(Proceso {
        nombre: "Edge".to_string(),
        tiempo_llegada: 1,
        rafaga: 4,
        tiempo_inicio: 0,
        tiempo_finalizacion: 0,
        tiempo_espera: 0,
    });
    procesos.push(Proceso {
        nombre: "Adobe".to_string(),
        tiempo_llegada: 2,
        rafaga: 3,
        tiempo_inicio: 0,
        tiempo_finalizacion: 0,
        tiempo_espera: 0,
    });
    
    // Ejecutar FCFS y mostrar resultados
    fcfs(&mut procesos);
    imprimir_resultados(&procesos);
}
