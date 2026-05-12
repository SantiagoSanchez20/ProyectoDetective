//
// Created by ASUS on 12/05/2026.
//

#include "Scorerecord.h"


ScoreRecord::ScoreRecord(const std::string& nombreDetective, int mejorPuntaje)
    : nombreDetective(nombreDetective), mejorPuntaje(mejorPuntaje),
      izquierda(nullptr), derecha(nullptr) {}

std::string ScoreRecord::getNombreDetective() const {
    return nombreDetective;
}

int ScoreRecord::getMejorPuntaje() const {
    return mejorPuntaje;
}

void ScoreRecord::setMejorPuntaje(int puntaje) {
    this->mejorPuntaje = puntaje;
}
