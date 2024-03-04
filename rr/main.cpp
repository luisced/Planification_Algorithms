#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <OpenXLSX.hpp>
using namespace std;
using namespace OpenXLSX;

// Estructura para representar los atributos de un proceso
struct Proceso
{
    std::string nombre;        // Nombre del proceso
    int tiempoRafaga;          // Tiempo de ráfaga (tiempo de ejecución)
    int tiempoLlegada;         // Tiempo de llegada del proceso
    int tiempoRestante;        // Tiempo restante para completar la ejecución
    int tiempoEspera;          // Tiempo de espera del proceso
    int tiempoRetorno;         // Tiempo de retorno del proceso
    int quantum;               // Quantum asignado para el algoritmo Round Robin
    int ordenTerminacion;      // Orden de terminación del proceso
};

// Función para implementar el algoritmo Round Robin
void roundRobin(std::vector<Proceso>& procesos, int quantum)
{
    std::queue<Proceso*> colaListos; // Cola para almacenar los procesos listos
    int tiempoActual = 0;              // Tiempo actual en la ejecución
    int procesosCompletados = 0;       // Contador de procesos completados
    int tiempoCambioContexto = 1;      // Tiempo de cambio de contexto
    int orden = 1;                     // Orden de terminación de los procesos

    // Bucle mientras no se hayan completado todos los procesos
    while (procesosCompletados < procesos.size())
    {
        // Añadir procesos a la cola de listos según su tiempo de llegada
        for (auto& p : procesos)
        {
            if (p.tiempoLlegada == tiempoActual)
            {
                colaListos.push(&p);
            }
        }

        // Si hay procesos listos para ejecución
        if (!colaListos.empty())
        {
            Proceso* procesoActual = colaListos.front(); // Obtener el primer proceso de la cola
            colaListos.pop(); // Sacar el proceso de la cola

            // Calcular el tiempo de ejecución para esta ráfaga
            int rebanadaTiempo = std::min(procesoActual->tiempoRestante, quantum);
            tiempoActual += rebanadaTiempo; // Actualizar el tiempo actual
            procesoActual->tiempoRestante -= rebanadaTiempo; // Actualizar el tiempo restante del proceso

            // Si el proceso ha completado su ejecución
            if (procesoActual->tiempoRestante == 0)
            {
                procesoActual->tiempoRetorno = tiempoActual - procesoActual->tiempoLlegada; // Calcular el tiempo de retorno
                procesoActual->tiempoEspera = procesoActual->tiempoRetorno - procesoActual->tiempoRafaga; // Calcular el tiempo de espera
                procesoActual->ordenTerminacion = orden++; // Asignar el orden de terminación
                procesosCompletados++; // Incrementar el contador de procesos completados
            }
            else
            {
                colaListos.push(procesoActual); // Volver a añadir el proceso a la cola si no se ha completado
            }

            // Simular el cambio de contexto si hay más procesos en la cola
            if (!colaListos.empty())
            {
                tiempoActual += tiempoCambioContexto;
            }
        }
        else
        {
            tiempoActual++; // Incrementar el tiempo actual si no hay procesos listos
        }
    }
}

// Función para mostrar las estadísticas de los procesos
void mostrarEstadisticas(const std::vector<Proceso>& procesos)
{
    double tiempoEsperaTotal = 0; // Acumulador para el tiempo total de espera
    double tiempoRetornoTotal = 0; // Acumulador para el tiempo total de retorno

    // Mostrar encabezados de las columnas
    std::cout << "\nNombre Proceso\tTiempo Ráfaga\tTiempo Llegada\tTiempo Espera\tTiempo Retorno\tOrden Terminación\n";
    // Iterar sobre los procesos y mostrar sus estadísticas
    for (const auto& p : procesos)
    {
        std::cout << p.nombre << "\t\t" << p.tiempoRafaga << "\t\t" << p.tiempoLlegada << "\t\t" << p.tiempoEspera << "\t\t" << p.tiempoRetorno << "\t\t" << p.ordenTerminacion << "\n";
        tiempoEsperaTotal += p.tiempoEspera;
        tiempoRetornoTotal += p.tiempoRetorno;
    }

    // Mostrar el tiempo de espera y el tiempo de retorno promedio
    std::cout << "\nTiempo Espera Promedio: " << tiempoEsperaTotal / procesos.size();
    std::cout << "\nTiempo Retorno Promedio: " << tiempoRetornoTotal / procesos.size() << "\n";
}

// Función para leer los datos de los procesos desde un archivo de Excel
vector<Proceso> leerExcel(const string& ruta)
{
    vector<Proceso> procesos; // Vector para almacenar los procesos

    XLDocument doc; // Crear un objeto para manejar el archivo de Excel
    doc.open(ruta); // Abrir el archivo de Excel
    auto hojaTrabajo = doc.workbook().worksheet("Hoja1"); // Seleccionar la hoja de trabajo

    int fila = 2; // Iniciar en la segunda fila, asumiendo que la primera fila contiene encabezados

    // Leer los datos de los procesos hasta encontrar una celda vacía
    try
    {
        while (hojaTrabajo.cell(XLCellReference("A" + to_string(fila))).value().type() != OpenXLSX::XLValueType::Empty)
        {
            Proceso proceso; // Crear un objeto Proceso
            proceso.nombre = hojaTrabajo.cell(XLCellReference("A" + to_string(fila))).value().get<std::string>(); // Leer el nombre del proceso
            proceso.tiempoLlegada = hojaTrabajo.cell(XLCellReference("B" + to_string(fila))).value().get<int>(); // Leer el tiempo de llegada
            proceso.tiempoRafaga = hojaTrabajo.cell(XLCellReference("C" + to_string(fila))).value().get<int>(); // Leer el tiempo de ráfaga
            proceso.quantum = hojaTrabajo.cell(XLCellReference("D2")).value().get<int>(); // Leer el quantum (asumiendo que es el mismo para todos los procesos)
            proceso.tiempoRestante = proceso.tiempoRafaga; // Inicializar el tiempo restante

            procesos.push_back(proceso); // Añadir el proceso al vector
            ++fila; // Pasar a la siguiente fila
        }
    }
    catch (const OpenXLSX::XLValueTypeError& e) // Capturar cualquier error al leer los datos
    {
        cerr << "Error: " << e.what() << endl;
    }

    doc.close(); // Cerrar el archivo de Excel
    return procesos; // Devolver el vector de procesos
}

int main()
{
    const char* ruta = "../Planification Algorithms.xlsx"; // Ruta del archivo de Excel
    std::vector<Proceso> procesos = leerExcel(ruta); // Leer los datos de los procesos
    roundRobin(procesos, procesos[0].quantum); // Ejecutar el algoritmo Round Robin
    mostrarEstadisticas(procesos); // Mostrar las estadísticas de los procesos

    return 0; // Indicar que el programa terminó correctamente
}
