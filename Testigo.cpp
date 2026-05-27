#include "Testigo.h"

Testigo::Testigo(const std::string& nombre, const std::string& declaracion)
    : nombre(nombre), declaracion(declaracion) {}

std::string Testigo::getNombre() const {
    return nombre;
}

std::string Testigo::getDeclaracion() const {
    return declaracion;
}

void Testigo::setDeclaracion(const std::string& dec) {
    declaracion = dec;
}