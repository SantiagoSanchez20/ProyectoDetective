//
// Created by ASUS on 12/05/2026.
//

#ifndef PROYECTODETECTIVE_MAPA_H
#define PROYECTODETECTIVE_MAPA_H

#include "Localizacion.h"
#include "Pista.h"
#include "Testigo.h"
#include <vector>

const int FILAS    = 11;
const int COLUMNAS = 11;

const int TOTAL_PISTAS     = 10;
const int TOTAL_CALLEJONES = 16;
const int TOTAL_TESTIGOS   = 5;

class Mapa {
private:
    Location* cabeza;
    Location* grilla[FILAS][COLUMNAS];

    std::vector<Pista*>    pistas;
    std::vector<Testigo*>  testigos;

    void construirLista();
    void colocarEdificios();
    void colocarCallejones();
    void colocarPistas();
    void colocarTestigos();

    Location* nodoLibreAleatorio();

public:
    Mapa();
    ~Mapa();

    Location* posicionInicialDetective();
    Location* getNodo(int fila, int columna) const;

    // Expuesto para que el detective pueda teletransportarse (Prueba Forense)
    Location* nodoParaTeletransporte();

    void imprimir(Location* posDetective) const;
    void cubrirMapa();
    void reubcarPista(Pista* pista);
    void eliminarDosCallejones();

    std::vector<Pista*>&   getPistas();
    std::vector<Testigo*>& getTestigos();
};

#endif //PROYECTODETECTIVE_MAPA_H