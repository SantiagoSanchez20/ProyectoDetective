//
// Created by ASUS on 12/05/2026.
//

#ifndef PROYECTODETECTIVE_SCORERECORD_H
#define PROYECTODETECTIVE_SCORERECORD_H


#pragma once
#include <string>

// Nodo del Árbol Binario de Búsqueda de puntajes históricos
class ScoreRecord {
private:
    std::string nombreDetective;
    int         mejorPuntaje;

public:
    // Punteros del ABB (públicos por diseño del TAD)
    ScoreRecord* izquierda;
    ScoreRecord* derecha;

    ScoreRecord(const std::string& nombreDetective, int mejorPuntaje);

    // Getters
    std::string getNombreDetective() const;
    int         getMejorPuntaje()    const;

    // Setter (actualizar puntaje si mejora)
    void setMejorPuntaje(int puntaje);
};



#endif //PROYECTODETECTIVE_SCORERECORD_H