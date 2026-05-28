//
// Created by ASUS on 12/05/2026.
//

#include "Mapa.h"

#include <iostream>
#include <cstdlib>   // rand(), srand()
#include <ctime>     // time()
#include <algorithm> // shuffle
#include <vector>

// ============================================================
//  CONSTRUCTOR
// ============================================================

Mapa::Mapa() : cabeza(nullptr) {
    // Semilla aleatoria para que cada partida sea diferente
    srand(static_cast<unsigned int>(time(nullptr)));

    // Paso 1: crear todos los nodos y enlazarlos
    construirLista();

    // Paso 2: marcar el borde exterior como edificios '#'
    colocarEdificios();

    // Paso 3: distribuir los 16 callejones en el interior
    colocarCallejones();

    // Paso 4: distribuir las 10 pistas en posiciones libres
    colocarPistas();

    // Paso 5: distribuir los 5 testigos en posiciones libres
    colocarTestigos();
}

// ============================================================
//  DESTRUCTOR
// ============================================================

Mapa::~Mapa() {
    // Recorremos la grilla y eliminamos cada nodo para liberar memoria
    for (int f = 0; f < FILAS; f++) {
        for (int c = 0; c < COLUMNAS; c++) {
            delete grilla[f][c];
        }
    }
    // Las pistas y testigos son eliminados por la clase Juego
}
//p
// ============================================================
//  construirLista
//  Crea los FILAS*COLUMNAS nodos y los conecta con 4 punteros
//  cada uno: arriba, abajo, izquierda, derecha
// ============================================================

void Mapa::construirLista() {
    // --- Fase 1: crear todos los nodos ---
    for (int f = 0; f < FILAS; f++) {
        for (int c = 0; c < COLUMNAS; c++) {
            // Cada nodo nace como NO_VISITADA; luego se especializa
            grilla[f][c] = new Location(f, c, TipoUbicacion::NO_VISITADA);
        }
    }

    // --- Fase 2: enlazar los nodos entre sí ---
    for (int f = 0; f < FILAS; f++) {
        for (int c = 0; c < COLUMNAS; c++) {
            Location* nodo = grilla[f][c];

            // Enlace hacia ARRIBA: existe si no estamos en la fila 0
            nodo->arriba    = (f > 0)            ? grilla[f-1][c] : nullptr;

            // Enlace hacia ABAJO: existe si no estamos en la última fila
            nodo->abajo     = (f < FILAS-1)      ? grilla[f+1][c] : nullptr;

            // Enlace hacia IZQUIERDA: existe si no estamos en la columna 0
            nodo->izquierda = (c > 0)            ? grilla[f][c-1] : nullptr;

            // Enlace hacia DERECHA: existe si no estamos en la última columna
            nodo->derecha   = (c < COLUMNAS-1)   ? grilla[f][c+1] : nullptr;
        }
    }

    // La cabeza de la lista es la esquina superior-izquierda
    cabeza = grilla[0][0];
}

// ============================================================
//  colocarEdificios
//  Los 4 bordes del tablero se marcan como EDIFICIO '#'
//  El detective no puede entrar en ellos (fila 0, fila 10,
//  columna 0, columna 10)
// ============================================================

void Mapa::colocarEdificios() {
    for (int f = 0; f < FILAS; f++) {
        for (int c = 0; c < COLUMNAS; c++) {
            // Es borde si está en la primera/última fila o columna
            bool esBorde = (f == 0 || f == FILAS-1 ||
                            c == 0 || c == COLUMNAS-1);
            if (esBorde) {
                grilla[f][c]->setTipo(TipoUbicacion::EDIFICIO);
                grilla[f][c]->setDescubierta(true); // los edificios siempre son visibles
            }
        }
    }
}

// ============================================================
//  colocarCallejones
//  Ubica TOTAL_CALLEJONES (16) callejones cerrados '|' en
//  posiciones interiores aleatorias y únicas
// ============================================================

void Mapa::colocarCallejones() {
    int colocados = 0;

    while (colocados < TOTAL_CALLEJONES) {
        // Fila y columna aleatorias del interior (1..9)
        int f = 1 + rand() % (FILAS - 2);
        int c = 1 + rand() % (COLUMNAS - 2);

        Location* nodo = grilla[f][c];

        // Solo se coloca si el nodo aún está libre (NO_VISITADA)
        if (nodo->getTipo() == TipoUbicacion::NO_VISITADA) {
            nodo->setTipo(TipoUbicacion::CALLEJON_CERRADO);
            colocados++;
        }
        // Si la posición ya estaba ocupada, se vuelve a intentar
    }
}

// ============================================================
//  colocarPistas
//  Crea las 10 pistas (distribuidas entre los 4 tipos) y las
//  ubica en posiciones interiores libres de manera aleatoria
// ============================================================

void Mapa::colocarPistas() {
    // Pool de 10 pistas: 3 Huellas, 3 Coartadas, 2 Testimonios, 2 Pruebas
    // (combinación libre según el enunciado; esta es una distribución razonable)
    std::vector<TipoPista> tipos = {
        TipoPista::HUELLA,         TipoPista::HUELLA,         TipoPista::HUELLA,
        TipoPista::COARTADA,       TipoPista::COARTADA,       TipoPista::COARTADA,
        TipoPista::TESTIMONIO,     TipoPista::TESTIMONIO,
        TipoPista::PRUEBA_FORENSE, TipoPista::PRUEBA_FORENSE
    };

    // Mezclamos para que el orden sea aleatorio en cada partida
    for (int i = (int)tipos.size() - 1; i > 0; i--) {
        int j = rand() % (i + 1);
        std::swap(tipos[i], tipos[j]);
    }

    int colocadas = 0;

    while (colocadas < TOTAL_PISTAS) {
        // Posición interior aleatoria
        int f = 1 + rand() % (FILAS - 2);
        int c = 1 + rand() % (COLUMNAS - 2);

        Location* nodo = grilla[f][c];

        // La pista solo se coloca en nodos completamente libres
        if (nodo->getTipo() == TipoUbicacion::NO_VISITADA) {
            // Creamos la pista y la asociamos al nodo
            Pista* p = new Pista(tipos[colocadas], "Pista encontrada en (" +
                                  std::to_string(f) + "," + std::to_string(c) + ")");
            nodo->setPista(p);           // el nodo pasa a tipo CON_PISTA
            pistas.push_back(p);         // la guardamos en el vector del mapa
            colocadas++;
        }
    }
}

// ============================================================
//  colocarTestigos
//  Ubica TOTAL_TESTIGOS (5) testigos 'W' en posiciones libres
//  (no pueden coincidir con pistas ni callejones)
// ============================================================

void Mapa::colocarTestigos() {
    // Nombres y declaraciones predefinidas
    // La declaración revela un atributo del culpable (se asignará en Juego)
    std::vector<std::string> nombres = {
        "Don Ramon", "La Vecina", "El Taxista", "La Enfermera", "El Estudiante"
    };

    int colocados = 0;

    while (colocados < TOTAL_TESTIGOS) {
        int f = 1 + rand() % (FILAS - 2);
        int c = 1 + rand() % (COLUMNAS - 2);

        Location* nodo = grilla[f][c];

        // Solo en nodos libres (sin pista, sin callejón, sin otro testigo)
        if (nodo->getTipo() == TipoUbicacion::NO_VISITADA) {
            // La declaración real se asignará en Juego cuando se conozca al culpable
            Testigo* t = new Testigo(nombres[colocados], "declaracion_pendiente");
            nodo->setTestigo(t);      // marca el nodo como CON_TESTIGO
            testigos.push_back(t);        // guardamos el testigo
            colocados++;
        }
    }
}

// ============================================================
//  posicionInicialDetective
//  Busca aleatoriamente un nodo interior libre para que el
//  detective aparezca al inicio de la partida
// ============================================================

Location* Mapa::posicionInicialDetective() {
    Location* nodo = nullptr;

    // Intentamos hasta encontrar un nodo completamente libre
    do {
        int f = 1 + rand() % (FILAS - 2);
        int c = 1 + rand() % (COLUMNAS - 2);
        nodo = grilla[f][c];
    } while (nodo->getTipo() != TipoUbicacion::NO_VISITADA);
    // El detective no puede aparecer sobre pistas, testigos ni callejones

    // Marcamos su posición inicial como descubierta y calle abierta
    nodo->setTipo(TipoUbicacion::CALLE_ABIERTA);
    nodo->setDescubierta(true);
    return nodo;
}

// ============================================================
//  nodoLibreAleatorio  (privado)
//  Devuelve un nodo interior que esté en estado NO_VISITADA
// ============================================================

Location* Mapa::nodoLibreAleatorio() {
    Location* nodo = nullptr;
    do {
        int f = 1 + rand() % (FILAS - 2);
        int c = 1 + rand() % (COLUMNAS - 2);
        nodo = grilla[f][c];
    } while (nodo->getTipo() != TipoUbicacion::NO_VISITADA);
    return nodo;
}

// ============================================================
//  nodoParaTeletransporte  (privado)
//  Devuelve un nodo interior que NO esté descubierto ni sea
//  callejón cerrado (usado por la Prueba Forense con 'X')
// ============================================================

Location* Mapa::nodoParaTeletransporte() {
    Location* nodo = nullptr;
    do {
        int f = 1 + rand() % (FILAS - 2);
        int c = 1 + rand() % (COLUMNAS - 2);
        nodo = grilla[f][c];
    } while (nodo->isDescubierta() ||
             nodo->getTipo() == TipoUbicacion::CALLEJON_CERRADO ||
             nodo->getTipo() == TipoUbicacion::EDIFICIO);
    return nodo;
}

// ============================================================
//  getNodo
//  Acceso directo a un nodo por coordenadas fila/columna
// ============================================================

Location* Mapa::getNodo(int fila, int columna) const {
    // Validamos que las coordenadas estén dentro del tablero
    if (fila < 0 || fila >= FILAS || columna < 0 || columna >= COLUMNAS) {
        return nullptr;
    }
    return grilla[fila][columna];
}

// ============================================================
//  imprimir
//  Recorre la lista desde la cabeza y dibuja el tablero
//  en consola fila por fila, columna por columna
// ============================================================

void Mapa::imprimir(Location* posDetective) const {
    std::cout << "\n";

    // Recorremos fila por fila usando el puntero 'abajo'
    Location* fila = cabeza; // empezamos desde la esquina superior-izquierda

    while (fila != nullptr) {
        // Recorremos la fila usando el puntero 'derecha'
        Location* actual = fila;

        while (actual != nullptr) {
            // Determinamos si este nodo es donde está el detective
            bool esDetective = (actual == posDetective);

            // getSimbolo() devuelve el carácter correcto según el estado del nodo
            std::cout << actual->getSimbolo(esDetective) << " ";

            actual = actual->derecha; // avanzamos a la derecha
        }

        std::cout << "\n";
        fila = fila->abajo; // bajamos a la siguiente fila
    }

    std::cout << "\n";
}

// ============================================================
//  cubrirMapa
//  Vuelve a tapar con 'o' todos los nodos del interior que
//  estaban descubiertos como CALLE_ABIERTA.
//  Se llama cuando el detective usa una pista con 'X'
// ============================================================

void Mapa::cubrirMapa() {
    for (int f = 1; f < FILAS - 1; f++) {
        for (int c = 1; c < COLUMNAS - 1; c++) {
            Location* nodo = grilla[f][c];

            // Solo cubrimos celdas abiertas (no callejones, pistas ni testigos)
            if (nodo->getTipo() == TipoUbicacion::CALLE_ABIERTA) {
                nodo->setTipo(TipoUbicacion::NO_VISITADA);
                nodo->setDescubierta(false);
            }
        }
    }
}

// ============================================================
//  reubcarPista
//  Mueve una pista a una posición aleatoria libre del mapa.
//  Se llama cuando el detective consume una pista con 'X'
// ============================================================

void Mapa::reubcarPista(Pista* pista) {
    // Buscamos un nodo libre donde reubicar la pista
    Location* destino = nodoLibreAleatorio();

    // Asociamos la pista al nuevo nodo
    destino->setPista(pista);
    // setPista() ya cambia el tipo del nodo a CON_PISTA internamente
}

// ============================================================
//  eliminarDosCallejones
//  Efecto de la Coartada: elimina 2 callejones aleatorios
//  del mapa (pueden ser visibles u ocultos)
// ============================================================

void Mapa::eliminarDosCallejones() {
    // Recolectamos todos los nodos que actualmente son callejones
    std::vector<Location*> callejones;

    for (int f = 1; f < FILAS - 1; f++) {
        for (int c = 1; c < COLUMNAS - 1; c++) {
            if (grilla[f][c]->getTipo() == TipoUbicacion::CALLEJON_CERRADO) {
                callejones.push_back(grilla[f][c]);
            }
        }
    }

    // Eliminamos hasta 2 callejones de forma aleatoria
    int aEliminar = std::min((int)callejones.size(), 2);

    for (int i = 0; i < aEliminar; i++) {
        // Escogemos un índice aleatorio dentro de los restantes
        int idx = rand() % callejones.size();

        // Convertimos el callejón en calle abierta
        callejones[idx]->setTipo(TipoUbicacion::CALLE_ABIERTA);
        callejones[idx]->setDescubierta(true);

        // Removemos del vector para no volver a elegirlo
        callejones.erase(callejones.begin() + idx);
    }
}

// ============================================================
//  GETTERS
// ============================================================

std::vector<Pista*>& Mapa::getPistas() {
    return pistas;
}

std::vector<Testigo*>& Mapa::getTestigos() {
    return testigos;
}