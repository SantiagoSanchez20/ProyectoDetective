#ifndef PROYECTODETECTIVE_TESTIGO_H
#define PROYECTODETECTIVE_TESTIGO_H
#include <string>

class Testigo {
private:
    std::string nombre;
    std::string declaracion;

public:
    Testigo(const std::string& nombre, const std::string& declaracion);

    // Getters
    std::string getNombre()      const;
    std::string getDeclaracion() const;

    // Setter
    void setDeclaracion(const std::string& dec);
};

#endif //PROYECTODETECTIVE_TESTIGO_H