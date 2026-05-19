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