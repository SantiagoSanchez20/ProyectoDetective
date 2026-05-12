cmake_minimum_required(VERSION 4.1)
project(ProyectoDetective)

set(CMAKE_CXX_STANDARD 20)

add_executable(ProyectoDetective main.cpp
        Pista.cpp
        Pista.h
        Localizacion.cpp
        Localizacion.h
        Testigo.cpp
        Testigo.h
        Sospechoso.cpp
        Sospechoso.h
        Detective.cpp
        Detective.h
        Scorerecord.cpp
        Scorerecord.h
        Mapa.cpp
        Mapa.h
        Tablahash.cpp
        Tablahash.h)
