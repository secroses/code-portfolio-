/*
 * ============================================================
 * PROYECTO: SIG-Conecta - Sistema de Gestion de Equipos
 * DESCRIPCION: Actividad Integradora - Persistencia de datos,
 *              estructuras, arreglos y modularidad en C++.
 * AUTOR: Edgar Yair Rosas Flores | Grupo 213 | UNRC
 * MATERIA: Programacion Estructurada | Semestre 2
 * FECHA: Abril 2026
 * ============================================================
 */

// --- LIBRERIAS ---
#include <iostream>   // Entrada/salida estandar (cin, cout)
#include <fstream>    // Manejo de archivos (ifstream, ofstream)
#include <string>     // Manejo de cadenas de texto (string, getline)
#include <limits>     // Para limpiar el buffer (numeric_limits)

using namespace std;

// ============================================================
// CONSTANTES
// ============================================================
const int MAX_EQUIPOS  = 20;   // Total de computadoras en el laboratorio
const int MAX_BITACORA = 100;  // Capacidad maxima de la bitacora en memoria

// ============================================================
// ESTRUCTURA: RegistroBitacora
// Almacena un incidente registrado en el sistema
// ============================================================
struct RegistroBitacora {
    int    idEquipo;       // Numero de equipo (1-20)
    string hora;           // Hora del incidente (ej. "14:30")
    string tipoIncidente;  // Tipo (ej. "Falla hardware")
    string descripcion;    // Descripcion breve del evento
};

// ============================================================
// ARREGLOS GLOBALES
// ============================================================
int equipos[MAX_EQUIPOS];               // 0=Libre, 1=Ocupada, 2=Mantenimiento
RegistroBitacora bitacora[MAX_BITACORA];
int totalRegistros = 0;

// ============================================================
// FUNCIONES AUXILIARES
// ============================================================

void inicializarEquipos() {
    for (int i = 0; i < MAX_EQUIPOS; i++) {
        equipos[i] = 0;
    }
}

void limpiarBuffer() {
    cin.clear();
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

void pausarPantalla() {
    cout << "\n[ Presiona Enter para volver al menu... ]";
    cin.ignore(numeric_limits<streamsize>::max(), '\n');
}

string obtenerEstado(int estado) {
    if (estado == 0) return "Libre";
    if (estado == 1) return "Ocupada";
    if (estado == 2) return "Mantenimiento";
    return "Desconocido";
}

// ============================================================
// FUNCION: cargarBitacora
// PROPOSITO: Persistencia - Input (ifstream)
// Distingue 4 casos al iniciar el sistema:
//   1. Archivo no existe       -> primera ejecucion
//   2. Archivo valido con datos -> recuperacion completa
//   3. Archivo valido sin datos -> sesion guardada limpia
//   4. Archivo invalido/corrupto -> advertencia, inicio limpio
// ============================================================
void cargarBitacora() {
    ifstream archivo("bitacora.txt");

    // CASO 1: El archivo NO existe (verdadera primera ejecucion)
    if (!archivo.is_open()) {
        cout << "[INFO] No se encontro 'bitacora.txt'. Se iniciara registro nuevo.\n";
        return;
    }

    string linea;
    string seccion      = "";
    bool   archivoValido = false;

    while (getline(archivo, linea)) {

        // Verificar que el archivo tiene el encabezado del sistema
        if (linea.find("BITACORA SIG-CONECTA") != string::npos) {
            archivoValido = true;
        }

        // Detectar cambio de seccion
        if (linea == "--- ESTADOS DE EQUIPOS ---") {
            seccion = "equipos";
            continue;
        }
        if (linea == "--- INCIDENTES REGISTRADOS ---") {
            seccion = "incidentes";
            continue;
        }

        // Saltar separadores, encabezados y lineas vacias
        if (linea.empty() || linea[0] == '=' || linea[0] == '-') continue;

        // ---- Seccion equipos: "EQUIPO_X: Estado" ----
        if (seccion == "equipos" && linea.size() > 7 && linea.substr(0, 7) == "EQUIPO_") {
            size_t pos = linea.find(": ");
            if (pos == string::npos) continue;

            int    idx    = stoi(linea.substr(7, pos - 7)) - 1;
            string estado = linea.substr(pos + 2);

            if      (estado == "Libre")         equipos[idx] = 0;
            else if (estado == "Ocupada")       equipos[idx] = 1;
            else if (estado == "Mantenimiento") equipos[idx] = 2;
        }

        // ---- Seccion incidentes: "CAMPO: valor" ----
        if (seccion == "incidentes" && totalRegistros < MAX_BITACORA) {
            size_t pos = linea.find(": ");
            if (pos == string::npos) continue;

            string etiqueta = linea.substr(0, pos);
            string valor    = linea.substr(pos + 2);

            if      (etiqueta == "EQUIPO")      { bitacora[totalRegistros].idEquipo      = stoi(valor); }
            else if (etiqueta == "HORA")        { bitacora[totalRegistros].hora          = valor; }
            else if (etiqueta == "TIPO")        { bitacora[totalRegistros].tipoIncidente = valor; }
            else if (etiqueta == "DESCRIPCION") { bitacora[totalRegistros].descripcion   = valor;
                                                  totalRegistros++;
                                                }
        }
    }

    archivo.close();

    // CASO 2: Archivo valido con incidentes previos
    if (archivoValido && totalRegistros > 0) {
        cout << "[+] Sesion anterior recuperada: "
             << totalRegistros << " incidente(s) cargados desde 'bitacora.txt'.\n";

    // CASO 3: Archivo valido pero sin incidentes
    } else if (archivoValido && totalRegistros == 0) {
        cout << "[INFO] 'bitacora.txt' encontrado. Sin incidentes previos. Sistema listo.\n";

    // CASO 4: Archivo existe pero no pertenece al sistema
    } else {
        cout << "[ADVERTENCIA] El archivo 'bitacora.txt' no tiene formato SIG-Conecta."
             << " Se iniciara registro nuevo.\n";
    }
}

// ============================================================
// FUNCION: guardarBitacora
// PROPOSITO: Persistencia - Output (ofstream)
// ============================================================
void guardarBitacora() {
    ofstream archivo("bitacora.txt");

    if (!archivo.is_open()) {
        cout << "[-] ERROR CRITICO: No se pudo crear 'bitacora.txt'.\n";
        return;
    }

    // Encabezado legible en Notepad
    archivo << "====================================================\n";
    archivo << "   BITACORA SIG-CONECTA: San Pedro de la Montana   \n";
    archivo << "====================================================\n\n";

    // Seccion 1: estados del arreglo int equipos[20]
    archivo << "--- ESTADOS DE EQUIPOS ---\n";
    for (int i = 0; i < MAX_EQUIPOS; i++) {
        archivo << "EQUIPO_" << (i + 1) << ": " << obtenerEstado(equipos[i]) << "\n";
    }

    // Seccion 2: arreglo de structs RegistroBitacora
    archivo << "\n--- INCIDENTES REGISTRADOS ---\n";

    if (totalRegistros == 0) {
        archivo << "(Sin incidentes registrados)\n";
    } else {
        for (int i = 0; i < totalRegistros; i++) {
            archivo << "\n";
            archivo << "EQUIPO: "      << bitacora[i].idEquipo      << "\n";
            archivo << "HORA: "        << bitacora[i].hora          << "\n";
            archivo << "TIPO: "        << bitacora[i].tipoIncidente << "\n";
            archivo << "DESCRIPCION: " << bitacora[i].descripcion   << "\n";
            archivo << "----------------------------------------------------\n";
        }
    }

    archivo << "\nTotal de incidentes: " << totalRegistros << "\n";
    archivo.close();

    cout << "[+] 'bitacora.txt' guardado correctamente ("
         << totalRegistros << " incidente(s)).\n";
}

// ============================================================
// OPCION 1: Consultar estado de equipos
// ============================================================
void consultarEstado() {
    cout << "\n--- Estado de los Equipos ---\n";

    int libres = 0, ocupadas = 0, enMantenimiento = 0;

    for (int i = 0; i < MAX_EQUIPOS; i++) {
        cout << "Equipo " << (i + 1) << ": " << obtenerEstado(equipos[i]) << "\n";
        if      (equipos[i] == 0) libres++;
        else if (equipos[i] == 1) ocupadas++;
        else                      enMantenimiento++;
    }

    cout << "\nResumen: "
         << libres          << " libre(s) | "
         << ocupadas        << " ocupada(s) | "
         << enMantenimiento << " en mantenimiento\n";

    pausarPantalla();
}

// ============================================================
// OPCION 2: Modificar estado de un equipo
// ============================================================
void modificarEstado() {
    int numEquipo, opcionEstado;

    cout << "\n--- Modificar Estado de Equipo ---\n";

    while (true) {
        cout << "Numero de equipo a modificar (1-20): ";
        cin >> numEquipo;
        if (cin.fail() || numEquipo < 1 || numEquipo > 20) {
            cout << "[ERROR] Numero invalido. Debe ser entre 1 y 20.\n";
            limpiarBuffer();
        } else {
            limpiarBuffer();
            break;
        }
    }

    cout << "Estado actual del Equipo " << numEquipo << ": "
         << obtenerEstado(equipos[numEquipo - 1]) << "\n";
    cout << "Nuevo estado:\n";
    cout << "  1. Libre\n";
    cout << "  2. Ocupada\n";
    cout << "  3. Mantenimiento\n";

    while (true) {
        cout << "Seleccione (1-3): ";
        cin >> opcionEstado;
        if (cin.fail() || opcionEstado < 1 || opcionEstado > 3) {
            cout << "[ERROR] Opcion invalida.\n";
            limpiarBuffer();
        } else {
            limpiarBuffer();
            break;
        }
    }

    equipos[numEquipo - 1] = opcionEstado - 1;
    cout << "[+] Equipo " << numEquipo << " -> "
         << obtenerEstado(equipos[numEquipo - 1]) << "\n";

    pausarPantalla();
}

// ============================================================
// OPCION 3: Registrar incidente
// ============================================================
void registrarIncidente() {
    if (totalRegistros >= MAX_BITACORA) {
        cout << "[-] La bitacora esta llena. Guarda y reinicia el sistema.\n";
        pausarPantalla();
        return;
    }

    cout << "\n--- Registrar Incidente ---\n";

    int idEquipo;

    while (true) {
        cout << "Equipo afectado (1-20): ";
        cin >> idEquipo;
        if (cin.fail() || idEquipo < 1 || idEquipo > 20) {
            cout << "[ERROR] Numero invalido.\n";
            limpiarBuffer();
        } else {
            limpiarBuffer();
            break;
        }
    }

    bitacora[totalRegistros].idEquipo = idEquipo;

    cout << "Hora del incidente (ej. 14:30): ";
    getline(cin, bitacora[totalRegistros].hora);

    cout << "Tipo de incidente (ej. Falla hardware, Error software): ";
    getline(cin, bitacora[totalRegistros].tipoIncidente);

    cout << "Descripcion breve: ";
    getline(cin, bitacora[totalRegistros].descripcion);

    totalRegistros++;
    cout << "[+] Incidente registrado. Total en bitacora: " << totalRegistros << "\n";

    pausarPantalla();
}

// ============================================================
// OPCION 4: Consultar historial
// ============================================================
void consultarHistorial() {
    cout << "\n--- Historial de Incidentes ---\n";

    if (totalRegistros == 0) {
        cout << "[INFO] No hay incidentes registrados en la bitacora.\n";
    } else {
        cout << "No. | Equipo   | Hora  | Tipo                | Descripcion\n";
        cout << "---------------------------------------------------------------\n";

        for (int i = 0; i < totalRegistros; i++) {
            cout << (i + 1)                            << "   | "
                 << "Equipo " << bitacora[i].idEquipo  << " | "
                 << bitacora[i].hora                   << " | "
                 << bitacora[i].tipoIncidente           << " | "
                 << bitacora[i].descripcion             << "\n";
        }
        cout << "\nTotal de registros: " << totalRegistros << "\n";
    }

    pausarPantalla();
}

// ============================================================
// FUNCION: mostrarMenu
// ============================================================
void mostrarMenu() {
    cout << "\n========================================\n";
    cout << "   SIG-CONECTA | San Pedro de la Montana\n";
    cout << "========================================\n";
    cout << "1. Consultar estado de equipos\n";
    cout << "2. Modificar estado de un equipo\n";
    cout << "3. Registrar incidente\n";
    cout << "4. Consultar historial (bitacora)\n";
    cout << "5. Guardar y salir\n";
    cout << "========================================\n";
    cout << "Elige una opcion: ";
}

// ============================================================
// FUNCION PRINCIPAL
// ============================================================
int main() {

    // Paso 1: inicializar todos los equipos en "Libre"
    inicializarEquipos();

    cout << "\n=== Iniciando SIG-Conecta v2.0 ===\n";

    // Paso 2: intentar recuperar sesion anterior desde archivo
    cargarBitacora();

    // Paso 3: ciclo principal del menu
    int  opcion;
    bool continuar = true;

    while (continuar) {
        mostrarMenu();
        cin >> opcion;

        if (cin.fail()) {
            limpiarBuffer();
            cout << "[ERROR] Ingresa un numero del 1 al 5.\n";
            continue;
        }
        limpiarBuffer();

        switch (opcion) {
            case 1: consultarEstado();    break;
            case 2: modificarEstado();    break;
            case 3: registrarIncidente(); break;
            case 4: consultarHistorial(); break;
            case 5:
                guardarBitacora();
                cout << "\nSistema cerrado correctamente. Hasta pronto.\n";
                continuar = false;
                break;
            default:
                cout << "[ERROR] Opcion no valida. Usa un numero del 1 al 5.\n";
        }
    }

    return 0;
}
