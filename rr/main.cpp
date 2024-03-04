#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <OpenXLSX.hpp>
using namespace std;
using namespace OpenXLSX;

struct Proceso
{
    std::string nombre;
    int tiempoRafaga;
    int tiempoLlegada;
    int tiempoRestante;
    int tiempoEspera;
    int tiempoRetorno;
    int quantum;
};

void roundRobin(std::vector<Proceso> &procesos, int quantum)
{
    std::queue<Proceso *> colaListos;
    int tiempoActual = 0;
    int procesosCompletados = 0;
    int tiempoCambioContexto = 1; // Asumiendo que el tiempo de cambio de contexto es 1 unidad

    // Añadir procesos a la cola de listos basándose en su tiempo de llegada
    while (procesosCompletados < procesos.size())
    {
        for (auto &p : procesos)
        {
            if (p.tiempoLlegada == tiempoActual)
            {
                colaListos.push(&p);
            }
        }

        if (!colaListos.empty())
        {
            Proceso *procesoActual = colaListos.front();
            colaListos.pop();

            int rebanadaTiempo = std::min(procesoActual->tiempoRestante, quantum);
            tiempoActual += rebanadaTiempo;
            procesoActual->tiempoRestante -= rebanadaTiempo;

            // Verificar si el proceso se ha completado
            if (procesoActual->tiempoRestante == 0)
            {
                procesoActual->tiempoRetorno = tiempoActual - procesoActual->tiempoLlegada;
                procesoActual->tiempoEspera = procesoActual->tiempoRetorno - procesoActual->tiempoRafaga;
                procesosCompletados++;
            }
            else
            {
                // Volver a añadir el proceso a la cola si no se ha completado
                colaListos.push(procesoActual);
            }

            // Simular cambio de contexto
            if (!colaListos.empty())
            {
                tiempoActual += tiempoCambioContexto;
            }
        }
        else
        {
            tiempoActual++;
        }
    }
}

void mostrarEstadisticas(const std::vector<Proceso> &procesos)
{
    double tiempoEsperaTotal = 0;
    double tiempoRetornoTotal = 0;

    std::cout << "\nNombre Proceso\tTiempo Ráfaga\tTiempo Llegada\tTiempo Espera\tTiempo Retorno\n";
    for (const auto &p : procesos)
    {
        std::cout << p.nombre << "\t\t" << p.tiempoRafaga << "\t\t" << p.tiempoLlegada << "\t\t" << p.tiempoEspera << "\t\t" << p.tiempoRetorno << "\n";
        tiempoEsperaTotal += p.tiempoEspera;
        tiempoRetornoTotal += p.tiempoRetorno;
    }

    std::cout << "\nTiempo Espera Promedio: " << tiempoEsperaTotal / procesos.size();
    std::cout << "\nTiempo Retorno Promedio: " << tiempoRetornoTotal / procesos.size() << "\n";
}
vector<Proceso> leerExcel(const string &ruta)
{
    vector<Proceso> procesos;

    XLDocument doc;
    doc.open(ruta);
    auto hojaTrabajo = doc.workbook().worksheet("Hoja1");

    // Assume that the first row has the headers and the data starts on the second row
    int fila = 2; // Row numbering starts at 1 in OpenXLSX

    try
    {
        while (hojaTrabajo.cell(XLCellReference("A" + to_string(fila))).value().type() != OpenXLSX::XLValueType::Empty)
        {
            Proceso proceso;
            proceso.nombre = hojaTrabajo.cell(XLCellReference("A" + to_string(fila))).value().get<std::string>();
            proceso.tiempoLlegada = hojaTrabajo.cell(XLCellReference("B" + to_string(fila))).value().get<int>();
            proceso.tiempoRafaga = hojaTrabajo.cell(XLCellReference("C" + to_string(fila))).value().get<int>();
            proceso.quantum = hojaTrabajo.cell(XLCellReference("D2")).value().get<int>();

            procesos.push_back(proceso);
            ++fila;
        }
    }
    catch (const OpenXLSX::XLValueTypeError &e)
    {
        cerr << "Error: " << e.what() << endl;
        // Handle the error, e.g., by returning an empty vector or logging the error.
    }

    doc.close();
    return procesos;
}

int main()
{
    const char *ruta = "../Planification Algorithms.xlsx";
    std::vector<Proceso> procesos = leerExcel(ruta);
    roundRobin(procesos, procesos[0].quantum);
    mostrarEstadisticas(procesos);

    return 0;
}