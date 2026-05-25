//
// Created by ASUS on 12/05/2026.
//

#ifndef PROYECTODETECTIVE_TABLAHASH_H
#define PROYECTODETECTIVE_TABLAHASH_H

//
#include "Sospechoso.h"
#include <unordered_map>
#include <string>
#include <vector>

class TablaHash {
private:
    // unordered_map es la implementación de Tabla Hash de la STL
    // La clave es el nombre del sospechoso (string)
    // El valor es el objeto Sospechoso completo
    // Búsqueda promedio O(1) gracias al hashing interno
    std::unordered_map<std::string, Sospechoso> tabla;

    // Guardamos el nombre del culpable para acceder rápido a él
    std::string nombreCulpable;

    // Pool completo de sospechosos predefinidos para escoger 8 al azar
    // Se define en el .cpp como datos estáticos
    static const std::vector<Sospechoso> POOL_SOSPECHOSOS;

public:
    // Constructor: llena la tabla con 8 sospechosos aleatorios del pool
    // y elige uno al azar como culpable
    TablaHash();

    // Inserta un sospechoso en la tabla usando su nombre como clave
    void insertar(const Sospechoso& sospechoso);

    // Busca un sospechoso por nombre — O(1) promedio
    // Devuelve un puntero al sospechoso, o nullptr si no existe
    Sospechoso* buscar(const std::string& nombre);

    // Revela el siguiente atributo del culpable y lo retorna
    // Se llama cada vez que el detective recoge una pista o interroga un testigo
    std::string revelarAtributoCulpable();

    // Devuelve el nombre del culpable (solo para uso interno del juego)
    std::string getNombreCulpable() const;

    // Muestra en pantalla la tabla con los atributos revelados hasta el momento
    // (tecla 'P' durante el juego)
    void mostrarRevelados() const;

    // Muestra en pantalla la tabla completa con todos los atributos
    // (fase de acusación al recoger las 10 pistas)
    void mostrarCompleta() const;

    // Devuelve todos los nombres de los sospechosos (para mostrar lista al acusar)
    std::vector<std::string> getNombres() const;

    // Devuelve el tamaño actual de la tabla
    int getTamanio() const;
};

#endif //PROYECTODETECTIVE_TABLAHASH_H