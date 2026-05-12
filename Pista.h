//
// Created by ASUS on 12/05/2026.
//

#ifndef PROYECTODETECTIVE_PISTA_H
#define PROYECTODETECTIVE_PISTA_H


#pragma once
#include <string>

enum class TipoPista {
    HUELLA,
    COARTADA,
    TESTIMONIO,
    PRUEBA_FORENSE
};

class Pista {
private:
    TipoPista   tipo;
    std::string descripcion;
    bool        usada;

public:
    Pista(TipoPista tipo, const std::string& descripcion);

    // Getters
    TipoPista   getTipo()        const;
    std::string getDescripcion() const;
    bool        isUsada()        const;

    // Setters
    void setUsada(bool usada);

    // Utilidades
    char        getSimbolo()    const;
    std::string getNombreTipo() const;
};

#endif //PROYECTODETECTIVE_PISTA_H