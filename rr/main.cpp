#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <OpenXLSX.hpp>
#include <iomanip>
using namespace std;
using namespace OpenXLSX;

// Estructura para representar los atributos de un proceso
struct Proceso
{
    std::string nombre;    // Nombre del proceso
    int tiempoRafaga;      // Tiempo de ráfaga (tiempo de ejecución)
    int tiempoLlegada;     // Tiempo de llegada del proceso
    int tiempoRestante;    // Tiempo restante para completar la ejecución
    int tiempoEspera = 0;  // Tiempo de espera del proceso
    int tiempoRetorno = 0; // Tiempo de retorno del proceso
    int quantum;           // Quantum asignado para el algoritmo Round Robin
    int ordenTerminacion;  // Orden de terminación del proceso
};

// Función para implementar el algoritmo Round Robin
void roundRobin(vector<Proceso> &procesos)
{
    queue<Proceso *> colaListos;
    int tiempoActual = 0;
    Proceso *procesoActual;

    // Asumimos que los procesos están ordenados por tiempo de llegada
    auto procesoIt = procesos.begin();

    // Procesamiento de los procesos
    while (procesoIt != procesos.end() || !colaListos.empty())
    {
        // Encolar procesos que han llegado
        while (procesoIt != procesos.end() && procesoIt->tiempoLlegada <= tiempoActual)
        {
            colaListos.push(&(*procesoIt));
            ++procesoIt;
        }

        // Si hay procesos en la cola, procesar el siguiente
        if (!colaListos.empty())
        {
            procesoActual = colaListos.front();
            colaListos.pop();

            // Calcular tiempo de ejecución para esta ráfaga
            int ejecucion = min(procesoActual->tiempoRestante, procesoActual->quantum);
            tiempoActual += ejecucion;
            procesoActual->tiempoRestante -= ejecucion;

            // Si el proceso terminó, calcular sus tiempos de espera y retorno
            if (procesoActual->tiempoRestante == 0)
            {
                procesoActual->tiempoRetorno = tiempoActual - procesoActual->tiempoLlegada;
                procesoActual->tiempoEspera = procesoActual->tiempoRetorno - procesoActual->tiempoRafaga;
            }
            else
            {
                // Si no terminó, volver a encolarlo
                colaListos.push(procesoActual);
            }
        }
        else
        {
            // Si no hay procesos listos, avanzar el tiempo
            tiempoActual++;
        }
    }
}

// Función para mostrar las estadísticas de los procesos
void imprimirResultados(const vector<Proceso> &procesos)
{
    // Imprimir encabezados de la tabla
    cout << left << setw(15) << "Proceso"
         << setw(15) << "T. Llegada"
         << setw(15) << "T. Rafaga"
         << setw(15) << "T. Espera"
         << setw(15) << "T. Retorno"
         << setw(15) << "Quantum" << endl;

    cout << string(90, '-') << endl; // Línea divisoria

    // Imprimir filas de la tabla
    for (const auto &p : procesos)
    {
        cout << left << setw(15) << p.nombre
             << setw(15) << p.tiempoLlegada
             << setw(15) << p.tiempoRafaga
             << setw(15) << p.tiempoEspera
             << setw(15) << p.tiempoRetorno
             << setw(15) << p.quantum << endl;
    }
}
// Función para leer los datos de los procesos desde un archivo de Excel
vector<Proceso> leerExcel(const string &ruta)
{
    vector<Proceso> procesos; // Vector para almacenar los procesos

    XLDocument doc;                                       // Crear un objeto para manejar el archivo de Excel
    doc.open(ruta);                                       // Abrir el archivo de Excel
    auto hojaTrabajo = doc.workbook().worksheet("Hoja1"); // Seleccionar la hoja de trabajo

    int fila = 2; // Iniciar en la segunda fila, asumiendo que la primera fila contiene encabezados

    // Leer los datos de los procesos hasta encontrar una celda vacía
    try
    {
        while (hojaTrabajo.cell(XLCellReference("A" + to_string(fila))).value().type() != OpenXLSX::XLValueType::Empty)
        {
            Proceso proceso;                                                                                      // Crear un objeto Proceso
            proceso.nombre = hojaTrabajo.cell(XLCellReference("A" + to_string(fila))).value().get<std::string>(); // Leer el nombre del proceso
            proceso.tiempoLlegada = hojaTrabajo.cell(XLCellReference("B" + to_string(fila))).value().get<int>();  // Leer el tiempo de llegada
            proceso.tiempoRafaga = hojaTrabajo.cell(XLCellReference("C" + to_string(fila))).value().get<int>();   // Leer el tiempo de ráfaga
            proceso.quantum = hojaTrabajo.cell(XLCellReference("D2")).value().get<int>();                         // Leer el quantum (asumiendo que es el mismo para todos los procesos)
            proceso.tiempoRestante = proceso.tiempoRafaga;                                                        // Inicializar el tiempo restante

            procesos.push_back(proceso); // Añadir el proceso al vector
            ++fila;                      // Pasar a la siguiente fila
        }
    }
    catch (const OpenXLSX::XLValueTypeError &e) // Capturar cualquier error al leer los datos
    {
        cerr << "Error: " << e.what() << endl;
    }

    doc.close();     // Cerrar el archivo de Excel
    return procesos; // Devolver el vector de procesos
}

int main()
{
    const char *ruta = "../Planification Algorithms.xlsx"; // Ruta del archivo de Excel
    std::vector<Proceso> procesos = leerExcel(ruta);
    roundRobin(procesos);
    imprimirResultados(procesos); // Mostrar las estadísticas de los procesos

    return 0; // Indicar que el programa terminó correctamente
}
