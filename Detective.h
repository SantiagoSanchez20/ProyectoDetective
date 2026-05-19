//
// Created by ASUS on 12/05/2026.
//
//p
#ifndef PROYECTODETECTIVE_DETECTIVE_H
#define PROYECTODETECTIVE_DETECTIVE_H


#include <string>
#include <stack>
#include <queue>
#include "Pista.h"
#include "Testigo.h"

// Forward declaration para evitar inclusión circular
class Location;

class Detective {
private:
    std::string nombre;
    int         puntaje;
    int         pistasRecogidas;
    Location*   posicion;

    std::stack<Pista*>   pilaPistas;    // TAD Pila (requisito 8)
    std::queue<Testigo*> colaTestigos;  // TAD Cola (requisito 17)

public:
    Detective(const std::string& nombre);

    // Getters
    std::string getNombre()         const;
    int         getPuntaje()        const;
    int         getPistasRecogidas() const;
    Location*   getPosicion()       const;

    // Setters
    void setPosicion(Location* posicion);

    // --- Lógica de movimiento ---
    void mover(); // incrementa el puntaje en 1

    // --- Lógica de puntaje ---
    void setPuntaje(int puntaje);
    void reducirPuntajeAMitad();
    void reducirPuntajeACero();
    void duplicarPuntaje();
    void penalizarPuntaje(); // lo duplica al perder el caso

    // --- Gestión de pistas (Pila) ---
    void   recogerPista(Pista* pista);
    Pista* usarPista();          // extrae el tope
    Pista* verUltimaPista() const;
    bool   tienePistas()    const;
    bool   casoCompleto()   const; // true cuando recogió 10 pistas

    // Imprime la pila de pistas en pantalla (tecla 'T')
    void mostrarPilas() const;

    // --- Gestión de testigos (Cola) ---
    void     agregarTestigo(Testigo* testigo);
    Testigo* interrogarTestigo(); // extrae el frente
    bool     tieneTestigos() const;
};


#endif //PROYECTODETECTIVE_DETECTIVE_H