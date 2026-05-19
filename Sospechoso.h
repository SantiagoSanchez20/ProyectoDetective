//
// Created by ASUS on 12/05/2026.
//

#ifndef PROYECTODETECTIVE_SOSPECHOSO_H
#define PROYECTODETECTIVE_SOSPECHOSO_H

#include <string>
#include <vector>
//p
class Sospechoso {
private:
    std::string nombre;
    std::string estatura;    // "alto", "bajo", "mediano"
    std::string cabello;     // "negro", "rubio", "rojo", "castaño"
    std::string piel;        // "clara", "morena", "oscura"
    std::string nariz;       // "aguileña", "chata", "recta"
    std::string sexo;        // "masculino", "femenino"
    std::string dominancia;  // "zurdo", "diestro"
    bool        culpable;

    // Atributos del culpable ya revelados al usuario (solo aplica si es culpable)
    std::vector<std::string> atributosRevelados;

public:
    Sospechoso();
    Sospechoso(const std::string& nombre,
               const std::string& estatura,
               const std::string& cabello,
               const std::string& piel,
               const std::string& nariz,
               const std::string& sexo,
               const std::string& dominancia);

    // Getters
    std::string getNombre()     const;
    std::string getEstatura()   const;
    std::string getCabello()    const;
    std::string getPiel()       const;
    std::string getNariz()      const;
    std::string getSexo()       const;
    std::string getDominancia() const;
    bool        isCulpable()    const;

    const std::vector<std::string>& getAtributosRevelados() const;

    // Setters
    void setCulpable(bool culpable);

    // Revela el siguiente atributo no revelado del culpable
    // Devuelve el atributo revelado, o "" si ya se revelaron todos
    std::string revelarAtributo();

    // Para mostrar en pantalla todos sus atributos (fase de acusación)
    std::string toStringCompleto()  const;

    // Para mostrar solo lo revelado durante el juego
    std::string toStringRevelado()  const;
};


#endif //PROYECTODETECTIVE_SOSPECHOSO_H