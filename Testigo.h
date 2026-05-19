//
// Created by ASUS on 12/05/2026.
//
//p
#ifndef PROYECTODETECTIVE_TESTIGO_H
#define PROYECTODETECTIVE_TESTIGO_H
#include <string>

class Testigo {
private:
    std::string nombre;
    std::string declaracion; // atributo del culpable que revela

public:
    Testigo(const std::string& nombre, const std::string& declaracion);

    // Getters
    std::string getNombre()      const;
    std::string getDeclaracion() const;
};


#endif //PROYECTODETECTIVE_TESTIGO_H