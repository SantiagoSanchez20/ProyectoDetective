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
    Sospechoso("Carlos",   "estatura - alto",    "cabello - negro",   "piel - morena", "nariz - aguilenna", "sexo - masculino", "mano - diestro"),
    Sospechoso("Diana",    "estatura - alto",    "cabello - rubio",   "piel - clara",  "nariz - recta",    "sexo - femenino",  "mano - zurdo"),
    Sospechoso("Eduardo",  "estatura - mediano", "cabello - castanno", "piel - clara",  "nariz - chata",    "sexo - masculino", "mano - diestro"),
    Sospechoso("Fernanda", "estatura - alto",    "cabello - rojo",    "piel - clara",  "nariz - recta",    "sexo - femenino",  "mano - diestro"),
    Sospechoso("Gonzalo",  "estatura - bajo",    "cabello - negro",   "piel - oscura", "nariz - aguilenna", "sexo - masculino", "mano - zurdo"),
    Sospechoso("Hilda",    "estatura - alto",    "cabello - castanno", "piel - morena", "nariz - chata",    "sexo - femenino",  "mano - diestro"),
    Sospechoso("Ivan",     "estatura - alto",    "cabello - rubio",   "piel - clara",  "nariz - recta",    "sexo - masculino", "mano - diestro"),
    Sospechoso("Julia",    "estatura - bajo",    "cabello - negro",   "piel - oscura", "nariz - chata",    "sexo - femenino",  "mano - zurdo"),
    Sospechoso("Kevin",    "estatura - mediano", "cabello - rojo",    "piel - clara",  "nariz - aguilenna", "sexo - masculino", "mano - diestro"),
    Sospechoso("Laura",    "estatura - alto",    "cabello - negro",   "piel - morena", "nariz - recta",    "sexo - femenino",  "mano - diestro"),
    Sospechoso("Miguel",   "estatura - alto",    "cabello - castanno", "piel - oscura", "nariz - chata",    "sexo - masculino", "mano - zurdo"),
    Sospechoso("Natalia",  "estatura - mediano", "cabello - rubio",   "piel - clara",  "nariz - aguilenna", "sexo - femenino",  "mano - diestro"),
    Sospechoso("Oscar",    "estatura - bajo",    "cabello - negro",   "piel - morena", "nariz - recta",    "sexo - masculino", "mano - diestro"),
    Sospechoso("Patricia", "estatura - alto",    "cabello - rojo",    "piel - oscura", "nariz - chata",    "sexo - femenino",  "mano - zurda")
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
    // Obtenemos los atributos ya revelados del culpable
    const Sospechoso& culpable = tabla.at(nombreCulpable);
    const std::vector<std::string>& revelados = culpable.getAtributosRevelados();

    for (const auto& par : tabla) {
        const Sospechoso& s = par.second;
        std::vector<std::string> coincidencias;

        // Checamos qué atributos revelados del culpable coinciden con este sospechoso
        for (const std::string& atr : revelados) {
            if (atr == s.getEstatura()   ||
                atr == s.getCabello()    ||
                atr == s.getPiel()       ||
                atr == s.getNariz()      ||
                atr == s.getSexo()       ||
                atr == s.getDominancia()) {
                coincidencias.push_back(atr);
                }
        }

        std::cout << "  " << s.getNombre() << " | atributos confirmados: ";
        if (coincidencias.empty()) {
            std::cout << "-";
        } else {
            for (size_t i = 0; i < coincidencias.size(); ++i) {
                std::cout << coincidencias[i];
                if (i + 1 < coincidencias.size()) std::cout << ", ";
            }
        }
        std::cout << "\n";
    }
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

    // Misma lógica que mostrarRevelados()
    const Sospechoso& culpable = tabla.at(nombreCulpable);
    const std::vector<std::string>& revelados = culpable.getAtributosRevelados();

    for (const auto& par : tabla) {
        const Sospechoso& s = par.second;
        std::vector<std::string> coincidencias;

        for (const std::string& atr : revelados) {
            if (atr == s.getEstatura()   ||
                atr == s.getCabello()    ||
                atr == s.getPiel()       ||
                atr == s.getNariz()      ||
                atr == s.getSexo()       ||
                atr == s.getDominancia()) {
                coincidencias.push_back(atr);
                }
        }

        std::cout << "  " << s.getNombre() << " | atributos confirmados: ";
        if (coincidencias.empty()) {
            std::cout << "-";
        } else {
            for (size_t i = 0; i < coincidencias.size(); ++i) {
                std::cout << coincidencias[i];
                if (i + 1 < coincidencias.size()) std::cout << ", ";
            }
        }
        std::cout << "\n";
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