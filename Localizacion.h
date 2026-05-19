//
// Created by ASUS on 12/05/2026.
//

#ifndef PROYECTODETECTIVE_LOCALIZACION_H
#define PROYECTODETECTIVE_LOCALIZACION_H


#include "Pista.h"

enum class TipoUbicacion {
    NO_VISITADA,      // 'o'
    CALLE_ABIERTA,    // ' '
    EDIFICIO,         // '#'
    CALLEJON_CERRADO, // '|'
    CON_PISTA,        // oculta, se muestra 'o' hasta ser recogida
    CON_TESTIGO       // 'W'
};

class Location {
private:
    int           fila;
    int           columna;
    TipoUbicacion tipo;
    bool          descubierta;
    Pista*        pista;    // nullptr si no hay pista
    bool          testigo;  // true si hay un testigo aquí

public:
    // Punteros de la lista múltiplemente enlazada (públicos por diseño del TAD)
    Location* arriba;
    Location* abajo;
    Location* izquierda;
    Location* derecha;

    Location(int fila, int columna,
             TipoUbicacion tipo = TipoUbicacion::NO_VISITADA);
    ~Location();

    // Getters
    int           getFila()        const;
    int           getColumna()     const;
    TipoUbicacion getTipo()        const;
    bool          isDescubierta()  const;
    Pista*        getPista()       const;
    bool          tieneTestigo()   const;
    //p
    // Setters
    void setTipo(TipoUbicacion tipo);
    void setDescubierta(bool descubierta);
    void setPista(Pista* pista);
    void setTestigo(bool testigo);

    // Utilidades
    char getSimbolo(bool esDetective) const;
};

#endif //PROYECTODETECTIVE_LOCALIZACION_H