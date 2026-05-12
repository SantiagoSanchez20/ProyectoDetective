//

//
//p
#include "Pista.h"

Pista::Pista(TipoPista tipo, const std::string& descripcion)
    : tipo(tipo), descripcion(descripcion), usada(false) {}

// --- Getters ---

TipoPista Pista::getTipo() const {
    return tipo;
}

std::string Pista::getDescripcion() const {
    return descripcion;
}

bool Pista::isUsada() const {
    return usada;
}

// --- Setters ---

void Pista::setUsada(bool usada) {
    this->usada = usada;
}

// --- Utilidades ---

char Pista::getSimbolo() const {
    switch (tipo) {
        case TipoPista::HUELLA:          return 'H';
        case TipoPista::COARTADA:        return 'C';
        case TipoPista::TESTIMONIO:      return 'T';
        case TipoPista::PRUEBA_FORENSE:  return 'P';
    }
    return '?';
}

std::string Pista::getNombreTipo() const {
    switch (tipo) {
        case TipoPista::HUELLA:          return "Huella";
        case TipoPista::COARTADA:        return "Coartada";
        case TipoPista::TESTIMONIO:      return "Testimonio";
        case TipoPista::PRUEBA_FORENSE:  return "Prueba Forense";
    }
    return "Desconocida";
}