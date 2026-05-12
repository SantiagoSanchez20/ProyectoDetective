//
// Created by ASUS on 12/05/2026.
//

#include "Sospechoso.h"
Sospechoso::Sospechoso()
    : culpable(false) {}
//p
Sospechoso::Sospechoso(const std::string& nombre,
                       const std::string& estatura,
                       const std::string& cabello,
                       const std::string& piel,
                       const std::string& nariz,
                       const std::string& sexo,
                       const std::string& dominancia)
    : nombre(nombre), estatura(estatura), cabello(cabello),
      piel(piel), nariz(nariz), sexo(sexo),
      dominancia(dominancia), culpable(false) {}

// --- Getters ---

std::string Sospechoso::getNombre()     const { return nombre;     }
std::string Sospechoso::getEstatura()   const { return estatura;   }
std::string Sospechoso::getCabello()    const { return cabello;    }
std::string Sospechoso::getPiel()       const { return piel;       }
std::string Sospechoso::getNariz()      const { return nariz;      }
std::string Sospechoso::getSexo()       const { return sexo;       }
std::string Sospechoso::getDominancia() const { return dominancia; }
bool        Sospechoso::isCulpable()    const { return culpable;   }

const std::vector<std::string>& Sospechoso::getAtributosRevelados() const {
    return atributosRevelados;
}

// --- Setters ---

void Sospechoso::setCulpable(bool culpable) {
    this->culpable = culpable;
}

// --- Utilidades ---

std::string Sospechoso::revelarAtributo() {
    // Lista ordenada de atributos a revelar progresivamente
    std::vector<std::string> todos = {
        estatura, cabello, piel, nariz, sexo, dominancia
    };

    size_t yaRevelados = atributosRevelados.size();
    if (yaRevelados >= todos.size()) return ""; // ya se reveló todo

    std::string nuevo = todos[yaRevelados];
    atributosRevelados.push_back(nuevo);
    return nuevo;
}

std::string Sospechoso::toStringCompleto() const {
    return nombre + " | estatura: " + estatura +
           ", cabello: "    + cabello    +
           ", piel: "       + piel       +
           ", nariz: "      + nariz      +
           ", sexo: "       + sexo       +
           ", dominancia: " + dominancia;
}

std::string Sospechoso::toStringRevelado() const {
    if (atributosRevelados.empty()) {
        return nombre + " | atributos confirmados: -";
    }

    std::string resultado = nombre + " | atributos confirmados: ";
    for (size_t i = 0; i < atributosRevelados.size(); ++i) {
        resultado += atributosRevelados[i];
        if (i + 1 < atributosRevelados.size()) resultado += ", ";
    }
    return resultado;
}
