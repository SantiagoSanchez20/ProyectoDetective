//
// Created by ASUS on 12/05/2026.
//

#include "Localizacion.h"




Location::Location(int fila, int columna, TipoUbicacion tipo)
    : fila(fila), columna(columna), tipo(tipo),
      descubierta(false), pista(nullptr), testigo(false),
      arriba(nullptr), abajo(nullptr),
      izquierda(nullptr), derecha(nullptr) {}
//p
Location::~Location() {
    // La pista es propiedad del mapa/juego, no se elimina aquí
}

// --- Getters ---

int Location::getFila() const {
    return fila;
}

int Location::getColumna() const {
    return columna;
}

TipoUbicacion Location::getTipo() const {
    return tipo;
}

bool Location::isDescubierta() const {
    return descubierta;
}

Pista* Location::getPista() const {
    return pista;
}

bool Location::tieneTestigo() const {
    return testigo;
}

// --- Setters ---

void Location::setTipo(TipoUbicacion tipo) {
    this->tipo = tipo;
}

void Location::setDescubierta(bool descubierta) {
    this->descubierta = descubierta;
}

void Location::setPista(Pista* pista) {
    this->pista = pista;
    if (pista != nullptr) {
        this->tipo = TipoUbicacion::CON_PISTA;
    }
}

void Location::setTestigo(bool testigo) {
    this->testigo = testigo;
    if (testigo) {
        this->tipo = TipoUbicacion::CON_TESTIGO;
    }
}

// --- Utilidades ---

char Location::getSimbolo(bool esDetective) const {
    if (esDetective) return 'I'; // 'I' de Investigador

    switch (tipo) {
        case TipoUbicacion::EDIFICIO:         return '#';
        case TipoUbicacion::CALLEJON_CERRADO: return '|';
        case TipoUbicacion::CALLE_ABIERTA:    return ' ';
        case TipoUbicacion::CON_TESTIGO:      return 'W';
        case TipoUbicacion::CON_PISTA:        // pista oculta
        case TipoUbicacion::NO_VISITADA:      return 'o';
    }
    return '?';
}