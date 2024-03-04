// Uso de bibliotecas para la lectura de archivos de Excel y la implementación del algoritmo Round Robin
#include <iostream>
#include <vector>
#include <queue>
#include <string>
#include <OpenXLSX.hpp>
using namespace std;
using namespace OpenXLSX;

//declaramos una estructura llamada Proceso en donde se le dan ciertos atributos a cada proceso
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
    std::queue<Proceso *> colaListos; // Creación de una cola para almacenar los procesos listos para ejecución.
    int tiempoActual = 0; // Inicialización del tiempo actual del sistema a 0.
    int procesosCompletados = 0; // Contador de los procesos que han completado su ejecución.
    int tiempoCambioContexto = 1; // Le dimos el valor de 1 al tiempo de cambio de contexto.

    
    while (procesosCompletados < procesos.size()) // Mientras no se hayan completado todos los procesos
    {
        for (auto &p : procesos) // Iterar sobre todos los procesos
        {
            if (p.tiempoLlegada == tiempoActual) //si el proceso ha llegado al tiempo actual, se añade a la cola de listos
            {
                colaListos.push(&p); // Añadir el proceso a la cola de listos
            }
        }

        if (!colaListos.empty()) // Si hay procesos listos para ejecución
        {
            Proceso *procesoActual = colaListos.front(); // Obtenemos el primer proceso de la cola
            colaListos.pop(); // Eliminamos el proceso de la cola

            int rebanadaTiempo = std::min(procesoActual->tiempoRestante, quantum); // Calculamos "rebanadaTiempo que se asignara al proceso actual"
            tiempoActual += rebanadaTiempo; // Actualizamos el tiempo actual del sistema
            procesoActual->tiempoRestante -= rebanadaTiempo; // Actualizamos el tiempo restante del proceso

            if (procesoActual->tiempoRestante == 0) // Si el proceso ha completado su ejecución
            {
                procesoActual->tiempoRetorno = tiempoActual - procesoActual->tiempoLlegada; // Calculamos el tiempo de retorno
                procesoActual->tiempoEspera = procesoActual->tiempoRetorno - procesoActual->tiempoRafaga; // Calculamos el tiempo de espera
                procesosCompletados++; // Incrementamos el contador de procesos completados
            }
            else
            {
                //Se vuelve a añadir el proceso a la cola si no se ha completado
                colaListos.push(procesoActual);
            }

         
            if (!colaListos.empty()) 
            {
                tiempoActual += tiempoCambioContexto; // Añadimos el tiempo de cambio de contexto si hay más procesos 
            }
        }
        else
        {
            tiempoActual++; // Incrementamos el tiempo actual si no hay procesos listos
        }
    }
}

void mostrarEstadisticas(const std::vector<Proceso> &procesos) // Función para mostrar las estadísticas de los procesos
{
    double tiempoEsperaTotal = 0; // Inicializamos del tiempo total de espera
    double tiempoRetornoTotal = 0; // Inicializamos del tiempo total de retorno

    std::cout << "\nNombre Proceso\tTiempo Ráfaga\tTiempo Llegada\tTiempo Espera\tTiempo Retorno\n"; // Mostrar encabezados
    for (const auto &p : procesos) // Iteramos sobre todos los procesos
    {
        std::cout << p.nombre << "\t\t" << p.tiempoRafaga << "\t\t" << p.tiempoLlegada << "\t\t" << p.tiempoEspera << "\t\t" << p.tiempoRetorno << "\n"; // Mostrar estadísticas de cada proceso
        tiempoEsperaTotal += p.tiempoEspera; // Calculamos el tiempo total de espera
        tiempoRetornoTotal += p.tiempoRetorno; // Calculamos el tiempo total de retorno
    }

    std::cout << "\nTiempo Espera Promedio: " << tiempoEsperaTotal / procesos.size(); // Mostramos el tiempo promedio de espera
    std::cout << "\nTiempo Retorno Promedio: " << tiempoRetornoTotal / procesos.size() << "\n"; // Mostramos el tiempo promedio de retorno
}
vector<Proceso> leerExcel(const string &ruta) // Utilizamos una función para leer los datos desde un archivo de Excel
{
    vector<Proceso> procesos; // Crear un vector para almacenar los procesos

    XLDocument doc; // Creamos un objeto de tipo XLDocument
    doc.open(ruta); // Abrimos el archivo de Excel
    auto hojaTrabajo = doc.workbook().worksheet("Hoja1"); // Seleccionamos la hoja de trabajo

    
    int fila = 2; // Inicializamos la fila a partir de la cual se encuentran los datos de los procesos

    try
    {
        while (hojaTrabajo.cell(XLCellReference("A" + to_string(fila))).value().type() != OpenXLSX::XLValueType::Empty) // Mientras no se llegue al final de los datos
        {
            Proceso proceso; // Se crea un objeto de tipo Proceso
            proceso.nombre = hojaTrabajo.cell(XLCellReference("A" + to_string(fila))).value().get<std::string>(); // Obtiene el nombre del proceso
            proceso.tiempoLlegada = hojaTrabajo.cell(XLCellReference("B" + to_string(fila))).value().get<int>(); // Obtiene el tiempo de llegada del proceso
            proceso.tiempoRafaga = hojaTrabajo.cell(XLCellReference("C" + to_string(fila))).value().get<int>(); // Obtiene el tiempo de ráfaga del proceso
            proceso.quantum = hojaTrabajo.cell(XLCellReference("D2")).value().get<int>();

            procesos.push_back(proceso); // Añade el proceso al vector de procesos
            ++fila; // Incrementa la fila
        }
    }
    catch (const OpenXLSX::XLValueTypeError &e) // Captura cualquier error que ocurra
    {
        cerr << "Error: " << e.what() << endl; // En todo caso muestra un mensaje de error
        

    doc.close(); // Cierra el archivo de Excel
    return procesos; // Devuelve el vector de procesos
    }
}

int main()   
{
    const char *ruta = "../Planification Algorithms.xlsx"; // Declaramos la ruta del archivo de Excel
    std::vector<Proceso> procesos = leerExcel(ruta); // Lee los datos de los procesos desde el archivo de Excel
    roundRobin(procesos, procesos[0].quantum); // Ejecuta el algoritmo de Round Robin
    mostrarEstadisticas(procesos); // Muestra las estadísticas de los procesos

    return 0; // Devuelve 0 si el programa se ejecutó correctamente
}