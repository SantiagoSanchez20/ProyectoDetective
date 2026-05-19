//
// Created by ASUS on 12/05/2026.
//

#ifndef PROYECTODETECTIVE_MAPA_H
#define PROYECTODETECTIVE_MAPA_H
//

#include "Localizacion.h"
#include "Pista.h"
#include "Testigo.h"
#include <vector>

// Dimensiones del tablero:
// 11x11 en total (9x9 interior + borde de edificios '#')
const int FILAS    = 11;
const int COLUMNAS = 11;

// Cantidades fijas del enunciado
const int TOTAL_PISTAS    = 10;
const int TOTAL_CALLEJONES = 16;
const int TOTAL_TESTIGOS   = 5;

class Mapa {
private:
    // Esquina superior-izquierda de la lista múltiplemente enlazada
    // Desde aquí se puede navegar a cualquier nodo siguiendo los punteros
    Location* cabeza;

    // Para imprimir el tablero necesitamos acceso rápido a cada nodo;
    // usamos un arreglo 2D de punteros (solo para impresión, no para jugabilidad)
    Location* grilla[FILAS][COLUMNAS];

    // Lista de pistas activas en el mapa (para reubicarlas al usar 'X')
    std::vector<Pista*> pistas;

    // Lista de testigos activos en el mapa
    std::vector<Testigo*> testigos;

    // --- Métodos privados de construcción ---

    // Crea todos los nodos y los enlaza entre sí (arriba/abajo/izq/der)
    void construirLista();

    // Marca el borde exterior como EDIFICIO '#'
    void colocarEdificios();

    // Ubica aleatoriamente los 16 callejones cerrados '|' en el interior
    void colocarCallejones();

    // Ubica aleatoriamente las 10 pistas en el interior
    // (no puede coincidir con callejones ni edificios)
    void colocarPistas();

    // Ubica aleatoriamente los 5 testigos en el interior
    // (no puede coincidir con pistas ni callejones)
    void colocarTestigos();

    // Devuelve un nodo interior aleatorio que cumpla una condición
    // Se usa para ubicar pistas, testigos, detective, etc.
    Location* nodoLibreAleatorio();

    // Devuelve un nodo aleatorio que NO esté descubierto ni sea callejón
    // (se usa para la Prueba Forense)
    Location* nodoParaTeletransporte();

public:
    // Constructor: construye la lista y coloca todos los elementos
    Mapa();

    // Destructor: libera todos los nodos de la lista
    ~Mapa();

    // Devuelve el nodo donde aparecerá el detective al inicio
    // (aleatorio, no puede ser pista, testigo ni callejón)
    Location* posicionInicialDetective();

    // Acceso a un nodo por coordenadas (usado en impresión y lógica)
    Location* getNodo(int fila, int columna) const;

    // Imprime el tablero completo en consola
    // posDetective: nodo donde está el detective ahora
    void imprimir(Location* posDetective) const;

    // Cubre todo el interior con 'o' (no visitado)
    // Se llama al usar una pista con 'X'
    void cubrirMapa();

    // Reubica una pista en una posición aleatoria libre
    // Se llama cuando el detective usa una pista con 'X'
    void reubcarPista(Pista* pista);

    // Elimina 2 callejones aleatorios del mapa
    // (efecto de la Coartada al usar 'X')
    void eliminarDosCallejones();

    // Getters
    std::vector<Pista*>&   getPistas();
    std::vector<Testigo*>& getTestigos();
};


#endif //PROYECTODETECTIVE_MAPA_H