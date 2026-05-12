//
// Created by ASUS on 12/05/2026.
//

#include "Tablahash.h"
#include <iostream>
#include <cstdlib>  // rand()
#include <ctime>    // time()
#include <algorithm> // shuffle

// ============================================================
//  POOL DE SOSPECHOSOS
//  Lista predefinida de 14 candidatos de los cuales se
//  elegirán 8 al azar al inicio de cada partida
// ============================================================

const std::vector<Sospechoso> TablaHash::POOL_SOSPECHOSOS = {
    // Sospechoso(nombre, estatura, cabello, piel, nariz, sexo, dominancia)
    Sospechoso("Carlos",   "alto",    "negro",   "morena", "aguileña", "masculino", "diestro"),
    Sospechoso("Diana",    "alta",    "rubio",   "clara",  "recta",    "femenino",  "zurda"),
    Sospechoso("Eduardo",  "mediano", "castaño", "clara",  "chata",    "masculino", "diestro"),
    Sospechoso("Fernanda", "alta",    "rojo",    "clara",  "recta",    "femenino",  "diestra"),
    Sospechoso("Gonzalo",  "bajo",    "negro",   "oscura", "aguileña", "masculino", "zurdo"),
    Sospechoso("Hilda",    "alta",    "castaño", "morena", "chata",    "femenino",  "diestra"),
    Sospechoso("Ivan",     "alto",    "rubio",   "clara",  "recta",    "masculino", "diestro"),
    Sospechoso("Julia",    "baja",    "negro",   "oscura", "chata",    "femenino",  "zurda"),
    Sospechoso("Kevin",    "mediano", "rojo",    "clara",  "aguileña", "masculino", "diestro"),
    Sospechoso("Laura",    "alta",    "negro",   "morena", "recta",    "femenino",  "diestra"),
    Sospechoso("Miguel",   "alto",    "castaño", "oscura", "chata",    "masculino", "zurdo"),
    Sospechoso("Natalia",  "mediana", "rubio",   "clara",  "aguileña", "femenino",  "diestra"),
    Sospechoso("Oscar",    "bajo",    "negro",   "morena", "recta",    "masculino", "diestro"),
    Sospechoso("Patricia", "alta",    "rojo",    "oscura", "chata",    "femenino",  "zurda")
};

// ============================================================
//  CONSTRUCTOR
//  Selecciona 8 sospechosos al azar del pool, los inserta en
//  la tabla hash y marca uno de ellos como culpable
// ============================================================

TablaHash::TablaHash() {
    // Creamos una copia del pool para poder barajarlo sin modificar el original
    std::vector<Sospechoso> candidatos = POOL_SOSPECHOSOS;

    // Algoritmo Fisher-Yates para mezclar aleatoriamente el pool
    for (int i = (int)candidatos.size() - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        std::swap(candidatos[i], candidatos[j]);
    }

    // Tomamos los primeros 8 del pool mezclado e insertamos en la tabla
    for (int i = 0; i < 8; i++) {
        insertar(candidatos[i]);
    }

    // Elegimos uno de los 8 sospechosos insertados como culpable al azar
    // Obtenemos todos los nombres de la tabla para seleccionar uno
    std::vector<std::string> nombres = getNombres();
    int indiceCulpable = rand() % nombres.size();
    nombreCulpable = nombres[indiceCulpable];

    // Marcamos ese sospechoso como culpable dentro de la tabla
    tabla[nombreCulpable].setCulpable(true);
}

// ============================================================
//  insertar
//  Agrega un sospechoso a la tabla usando su nombre como clave
//  Si ya existe una clave igual, la sobreescribe (no debería pasar
//  porque el pool tiene nombres únicos)
// ============================================================

void TablaHash::insertar(const Sospechoso& sospechoso) {
    // El operador [] del unordered_map realiza la inserción directa
    // La función hash de la STL convierte el nombre (string) en índice
    tabla[sospechoso.getNombre()] = sospechoso;
}
//p
// ============================================================
//  buscar
//  Busca un sospechoso por nombre en la tabla hash
//  Complejidad: O(1) promedio gracias al hashing
//  Devuelve puntero al sospechoso o nullptr si no existe
// ============================================================

Sospechoso* TablaHash::buscar(const std::string& nombre) {
    // find() del unordered_map hace la búsqueda por hash — O(1) promedio
    auto it = tabla.find(nombre);

    if (it != tabla.end()) {
        // El sospechoso fue encontrado; devolvemos dirección del objeto
        return &(it->second);
    }

    // No se encontró ningún sospechoso con ese nombre
    return nullptr;
}

// ============================================================
//  revelarAtributoCulpable
//  Llama a revelarAtributo() del culpable para mostrar un dato
//  nuevo al detective. Se usa cada vez que recoge una pista
//  o interroga un testigo
// ============================================================

std::string TablaHash::revelarAtributoCulpable() {
    // Buscamos al culpable directamente por su nombre (O(1))
    Sospechoso* culpable = buscar(nombreCulpable);

    if (culpable == nullptr) return ""; // no debería pasar

    // Pedimos al sospechoso que revele su siguiente atributo oculto
    // revelarAtributo() lleva internamente la cuenta de lo ya revelado
    std::string atributo = culpable->revelarAtributo();

    if (atributo.empty()) {
        return "No hay mas atributos por revelar.";
    }

    return atributo;
}

// ============================================================
//  mostrarRevelados
//  Imprime la tabla con SOLO los atributos que ya se revelaron
//  del culpable. Para los demás sospechosos muestra '-'
//  Se activa con la tecla 'S' durante el juego
// ============================================================

void TablaHash::mostrarRevelados() const {
    std::cout << "\n+-----------------------------------------+\n";
    std::cout << "|       SOSPECHOSOS DEL CASO              |\n";
    std::cout << "| (atributos del culpable revelados)      |\n";
    std::cout << "+-----------------------------------------+\n";

    // Recorremos todos los pares (clave, valor) de la tabla
    for (const auto& par : tabla) {
        // par.first  = nombre (clave)
        // par.second = objeto Sospechoso (valor)
        const Sospechoso& s = par.second;

        // toStringRevelado() muestra solo los atributos ya descubiertos
        std::cout << "  " << s.toStringRevelado() << "\n";
    }

    std::cout << "+-----------------------------------------+\n\n";
}

// ============================================================
//  mostrarCompleta
//  Imprime la tabla con TODOS los atributos de cada sospechoso
//  Se usa en la fase de acusación final
// ============================================================

void TablaHash::mostrarCompleta() const {
    std::cout << "\n+--------------------------------------------------+\n";
    std::cout << "|          SOSPECHOSOS - FASE DE ACUSACION         |\n";
    std::cout << "+--------------------------------------------------+\n";

    for (const auto& par : tabla) {
        const Sospechoso& s = par.second;

        // toStringRevelado() muestra lo que el detective descubrió hasta ahora
        std::cout << "  " << s.toStringRevelado() << "\n";
    }

    std::cout << "+--------------------------------------------------+\n\n";
}

// ============================================================
//  getNombres
//  Devuelve un vector con los nombres de todos los sospechosos
//  en la tabla. Se usa para mostrar la lista al momento de acusar
// ============================================================

std::vector<std::string> TablaHash::getNombres() const {
    std::vector<std::string> nombres;

    // Recorremos la tabla y extraemos solo las claves (nombres)
    for (const auto& par : tabla) {
        nombres.push_back(par.first);
    }

    return nombres;
}

// ============================================================
//  getNombreCulpable
//  Devuelve el nombre del culpable (uso interno del juego)
// ============================================================

std::string TablaHash::getNombreCulpable() const {
    return nombreCulpable;
}

// ============================================================
//  getTamanio
//  Devuelve cuántos sospechosos hay en la tabla actualmente
// ============================================================

int TablaHash::getTamanio() const {
    return (int)tabla.size();
}