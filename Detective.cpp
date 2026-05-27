//
// Created by ASUS on 12/05/2026.
//

#include "Detective.h"

#include <iostream>
#include <stack>
//
Detective::Detective(const std::string& nombre)
    : nombre(nombre), puntaje(0), pistasRecogidas(0), posicion(nullptr) {}

// Getters

std::string Detective::getNombre() const {
    return nombre;
}

int Detective::getPuntaje() const {
    return puntaje;
}

int Detective::getPistasRecogidas() const {
    return pistasRecogidas;
}

Location* Detective::getPosicion() const {
    return posicion;
}

// Setters

void Detective::setPosicion(Location* posicion) {
    this->posicion = posicion;
}

// Movimiento

void Detective::mover() {
    puntaje++;
}

// Puntaje

void Detective::setPuntaje(int puntaje) {
    this->puntaje = puntaje;
}

void Detective::reducirPuntajeAMitad() {
    puntaje = puntaje / 2;
}

void Detective::reducirPuntajeACero() {
    puntaje = 0;
}

void Detective::duplicarPuntaje() {
    puntaje = puntaje * 2;
}

void Detective::penalizarPuntaje() {
    puntaje = puntaje * 2;
}

// Pistas (Pila)

void Detective::recogerPista(Pista* pista) { //Una simple pila para guardar las pistas
    pilaPistas.push(pista);
    pistasRecogidas++;
}

Pista* Detective::usarPista() { //Se usan en estilo LIFO con un top y un pop.
    if (pilaPistas.empty()) return nullptr;
    Pista* p = pilaPistas.top();
    pilaPistas.pop();
    pistasRecogidas--;
    return p;
}

Pista* Detective::verUltimaPista() const { //Nuevamente un acceso LIFO al tope con top
    if (pilaPistas.empty()) return nullptr;
    return pilaPistas.top();
}

bool Detective::tienePistas() const {
    return !pilaPistas.empty();
}

bool Detective::casoCompleto() const {
    return pistasRecogidas >= 10;
}

void Detective::mostrarPilas() const {
    std::cout << "\n" << nombre << ", mira las pistas que llevas:\n";

    if (pilaPistas.empty()) {
        std::cout << "  [ sin pistas recolectadas ]\n";
        return;
    }

    // Copiamos la pila para poder recorrerla sin destruirla
    std::stack<Pista*> copia = pilaPistas;
    std::vector<Pista*> temp;

    while (!copia.empty()) {
        temp.push_back(copia.top());
        copia.pop();
    }

    // temp[0] = tope (ultima pista), temp[n-1] = primera recogida
    std::cout << "  +----------+\n";
    for (size_t i = 0; i < temp.size(); ++i) {
        std::string etiqueta = "";
        if (i == 0)                      etiqueta = " <- ultima (se usa con X)";
        else if (i == 1)                 etiqueta = " <- penultima";
        else if (i == 2)                 etiqueta = " <- antepenultima";

        std::cout << "  | " << temp[i]->getSimbolo()
                  << " - " << temp[i]->getNombreTipo()
                  << " |" << etiqueta << "\n";
    }
    std::cout << "  +----------+\n";
}

// --- Gestión de testigos (Cola) ---

void Detective::agregarTestigo(Testigo* testigo) { //Entran con FIFO a una cola
    colaTestigos.push(testigo);
}

Testigo* Detective::interrogarTestigo() { //Se interrogan nuevamente en FIFO
    if (colaTestigos.empty()) return nullptr;
    Testigo* t = colaTestigos.front();
    colaTestigos.pop();
    return t;
}

bool Detective::tieneTestigos() const {
    return !colaTestigos.empty();
}