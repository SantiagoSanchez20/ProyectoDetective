#include <iostream>

#include <iostream>
#include <string>
#include <limits>
#include <cstdlib>
#include <ctime>

#include "Mapa.h"
#include "Detective.h"
#include "Tablahash.h"
#include "Scorerecord.h"


// Insertar / actualizar  (ABB)
ScoreRecord* abbInsertar(ScoreRecord* raiz,
                         const std::string& nombre,
                         int puntaje) {
    if (raiz == nullptr) {
        return new ScoreRecord(nombre, puntaje);
    }

    if (nombre < raiz->getNombreDetective()) {
        raiz->izquierda = abbInsertar(raiz->izquierda, nombre, puntaje);
    } else if (nombre > raiz->getNombreDetective()) {
        raiz->derecha   = abbInsertar(raiz->derecha,   nombre, puntaje);
    } else {
        // Si el detectiva ya existe, se conserva el puntaje menor.
        if (puntaje < raiz->getMejorPuntaje()) {
            raiz->setMejorPuntaje(puntaje);
            std::cout << "  [ABB] Nuevo mejor puntaje de " << nombre
                      << ": " << puntaje << " movimientos.\n";
        } else {
            std::cout << "  [ABB] " << nombre << " ya tenia un mejor puntaje ("
                      << raiz->getMejorPuntaje() << "). No se actualiza.\n";
        }
    }
    return raiz;
}

// Buscar (requerimiento 22)
ScoreRecord* abbBuscar(ScoreRecord* raiz, const std::string& nombre) {
    if (raiz == nullptr) return nullptr;
    if (nombre == raiz->getNombreDetective()) return raiz;
    if (nombre  < raiz->getNombreDetective()) return abbBuscar(raiz->izquierda, nombre);
    return abbBuscar(raiz->derecha, nombre);
}

// Recorrido en orden: menor puntaje primero (requerimiento 23)
//  El ABB está ordenado por nombre, para ordenar por puntaje se mete en un vector y ordenamos, asi no se cambia el ABB
void abbRecolectar(ScoreRecord* raiz,
                   std::vector<ScoreRecord*>& vec) {
    if (raiz == nullptr) return;
    abbRecolectar(raiz->izquierda, vec);
    vec.push_back(raiz);
    abbRecolectar(raiz->derecha, vec);
}

void abbMostrarRanking(ScoreRecord* raiz) {
    std::vector<ScoreRecord*> vec;
    abbRecolectar(raiz, vec);

    if (vec.empty()) {
        std::cout << "  (sin partidas registradas aun)\n";
        return;
    }

    // Ordenar por puntaje ascendente (mejor = menos movimientos)
    for (size_t i = 0; i < vec.size(); ++i) {
        for (size_t j = i + 1; j < vec.size(); ++j) {
            if (vec[j]->getMejorPuntaje() < vec[i]->getMejorPuntaje()) {
                std::swap(vec[i], vec[j]);
            }
        }
    }

    std::cout << "\n+------------------------------------+\n";
    std::cout << "|   RANKING HISTORICO DE DETECTIVES  |\n";
    std::cout << "+------------------------------------+\n";
    int pos = 1;
    for (auto* r : vec) {
        std::cout << "  " << pos++ << ". "
                  << r->getNombreDetective()
                  << "  ->  " << r->getMejorPuntaje()
                  << " movimientos\n";
    }
    std::cout << "+------------------------------------+\n\n";
}