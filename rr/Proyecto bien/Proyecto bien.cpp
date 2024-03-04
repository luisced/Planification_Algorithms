#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include "libxl.h"
#include <xls.h>
using namespace std;



struct Proceso {
    std::string nombre;
    int tiempoRafaga;
    int tiempoLlegada;
    int tiempoRestante;
    int tiempoEspera;
    int tiempoRetorno;
};




void roundRobin(std::vector<Proceso>& procesos, int quantum) {
    std::queue<Proceso*> colaListos;
    int tiempoActual = 0;
    int procesosCompletados = 0;
    int tiempoCambioContexto = 1; // Asumiendo que el tiempo de cambio de contexto es 1 unidad

    // Añadir procesos a la cola de listos basándose en su tiempo de llegada
    while (procesosCompletados < procesos.size()) {
        for (auto& p : procesos) {
            if (p.tiempoLlegada == tiempoActual) {
                colaListos.push(&p);
            }
        }

        if (!colaListos.empty()) {
            Proceso* procesoActual = colaListos.front();
            colaListos.pop();

            int rebanadaTiempo = std::min(procesoActual->tiempoRestante, quantum);
            tiempoActual += rebanadaTiempo;
            procesoActual->tiempoRestante -= rebanadaTiempo;

            // Verificar si el proceso se ha completado
            if (procesoActual->tiempoRestante == 0) {
                procesoActual->tiempoRetorno = tiempoActual - procesoActual->tiempoLlegada;
                procesoActual->tiempoEspera = procesoActual->tiempoRetorno - procesoActual->tiempoRafaga;
                procesosCompletados++;
            }
            else {
                // Volver a añadir el proceso a la cola si no se ha completado
                colaListos.push(procesoActual);
            }

            // Simular cambio de contexto
            if (!colaListos.empty()) {
                tiempoActual += tiempoCambioContexto;
            }
        }
        else {
            tiempoActual++;
        }
    }
}

void mostrarEstadisticas(const std::vector<Proceso>& procesos) {
    double tiempoEsperaTotal = 0;
    double tiempoRetornoTotal = 0;

    std::cout << "\nNombre Proceso\tTiempo Ráfaga\tTiempo Llegada\tTiempo Espera\tTiempo Retorno\n";
    for (const auto& p : procesos) {
        std::cout << p.nombre << "\t\t" << p.tiempoRafaga << "\t\t" << p.tiempoLlegada << "\t\t" << p.tiempoEspera << "\t\t" << p.tiempoRetorno << "\n";
        tiempoEsperaTotal += p.tiempoEspera;
        tiempoRetornoTotal += p.tiempoRetorno;
    }

    std::cout << "\nTiempo Espera Promedio: " << tiempoEsperaTotal / procesos.size();
    std::cout << "\nTiempo Retorno Promedio: " << tiempoRetornoTotal / procesos.size() << "\n";
}
std::vector<Proceso> leerExcel(const char* ruta) {
    xlsWorkBook* libroTrabajo;
    xlsWorkSheet* hojaTrabajo;
    libroTrabajo = xls_open(ruta, "UTF-8");
    if (!libroTrabajo) {
        std::cerr << "No se pudo abrir el archivo Excel." << std::endl;
        exit(1);
    }

    hojaTrabajo = xls_getWorkSheet(libroTrabajo, 0); // Hoja 1
    if (!hojaTrabajo) {
        std::cerr << "No se pudo obtener la hoja de cálculo." << std::endl;
        xls_close(libroTrabajo);
        exit(1);
    }

    xls_parseWorkSheet(hojaTrabajo);

    std::vector<Proceso> procesos;

    for (int i = 0; i <= hojaTrabajo->rows.lastrow; i++) {
        Proceso proceso;
        proceso.nombre = hojaTrabajo->rows.row[i].cells.cell[0]->str;
        proceso.tiempoLlegada = hojaTrabajo->rows.row[i].cells.cell[1]->d;
        proceso.tiempoRafaga = hojaTrabajo->rows.row[i].cells.cell[2]->d;
        proceso.tiempoRestante = proceso.tiempoRafaga;
        proceso.tiempoEspera = 0;
        proceso.tiempoRetorno = 0;
        procesos.push_back(proceso);
    }

    xls_close(libroTrabajo);

    return procesos;
}


int main() {
    const char* ruta = "Planification Algorithms.xls";
    std::vector<Proceso> procesos = leerExcel(ruta);
    int quantum = 4; // El quantum podría ser leído del archivo Excel si es necesario.

    roundRobin(procesos, quantum);
    mostrarEstadisticas(procesos);

    return 0;
}