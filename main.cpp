#include <iostream>

#include <iostream>
#include <string>
#include <limits>
#include <cstdlib>
#include <ctime>

#include "Mapa.h"
#include "Detective.h"
#include "Tablahash.h"
#include "Scorerecord.h"


// Insertar / actualizar  (ABB)
ScoreRecord* abbInsertar(ScoreRecord* raiz,
                         const std::string& nombre,
                         int puntaje) {
    if (raiz == nullptr) {
        return new ScoreRecord(nombre, puntaje);
    }

    if (nombre < raiz->getNombreDetective()) {
        raiz->izquierda = abbInsertar(raiz->izquierda, nombre, puntaje);
    } else if (nombre > raiz->getNombreDetective()) {
        raiz->derecha   = abbInsertar(raiz->derecha,   nombre, puntaje);
    } else {
        // Si el detectiva ya existe, se conserva el puntaje menor.
        if (puntaje < raiz->getMejorPuntaje()) {
            raiz->setMejorPuntaje(puntaje);
            std::cout << "  [ABB] Nuevo mejor puntaje de " << nombre
                      << ": " << puntaje << " movimientos.\n";
        } else {
            std::cout << "  [ABB] " << nombre << " ya tenia un mejor puntaje ("
                      << raiz->getMejorPuntaje() << "). No se actualiza.\n";
        }
    }
    return raiz;
}

// Buscar (requerimiento 22)
ScoreRecord* abbBuscar(ScoreRecord* raiz, const std::string& nombre) {
    if (raiz == nullptr) return nullptr;
    if (nombre == raiz->getNombreDetective()) return raiz;
    if (nombre  < raiz->getNombreDetective()) return abbBuscar(raiz->izquierda, nombre);
    return abbBuscar(raiz->derecha, nombre);
}

// Recorrido en orden: menor puntaje primero (requerimiento 23)
//  El ABB está ordenado por nombre, para ordenar por puntaje se mete en un vector y ordenamos, asi no se cambia el ABB
void abbRecolectar(ScoreRecord* raiz,
                   std::vector<ScoreRecord*>& vec) {
    if (raiz == nullptr) return;
    abbRecolectar(raiz->izquierda, vec);
    vec.push_back(raiz);
    abbRecolectar(raiz->derecha, vec);
}

void abbMostrarRanking(ScoreRecord* raiz) {
    std::vector<ScoreRecord*> vec;
    abbRecolectar(raiz, vec);

    if (vec.empty()) {
        std::cout << "  (sin partidas registradas aun)\n";
        return;
    }

    // Ordenar por puntaje ascendente (mejor = menos movimientos)
    for (size_t i = 0; i < vec.size(); ++i) {
        for (size_t j = i + 1; j < vec.size(); ++j) {
            if (vec[j]->getMejorPuntaje() < vec[i]->getMejorPuntaje()) {
                std::swap(vec[i], vec[j]);
            }
        }
    }

    std::cout << "\n+------------------------------------+\n";
    std::cout << "|   RANKING HISTORICO DE DETECTIVES  |\n";
    std::cout << "+------------------------------------+\n";
    int pos = 1;
    for (auto* r : vec) {
        std::cout << "  " << pos++ << ". "
                  << r->getNombreDetective()
                  << "  ->  " << r->getMejorPuntaje()
                  << " movimientos\n";
    }
    std::cout << "+------------------------------------+\n\n";
}
// ── Limpiar la memoria del Árbol Binario de Búsqueda (ABB) ──
// Usamos un recorrido Post-Orden (Izquierda -> Derecha -> Raíz)
// para no dejar nodos huérfanos al borrar.
void abbLiberar(ScoreRecord* raiz) {
    if (raiz == nullptr) return; // Caso base: si el nodo está vacío, paramos.

    abbLiberar(raiz->izquierda); // Primero va a fondo por la izquierda
    abbLiberar(raiz->derecha);   // Luego a fondo por la derecha
    delete raiz;                 // Cuando los hijos ya no existen, borra la raíz actual
}

// ============================================================
//  HERRAMIENTAS VISUALES Y DE ENTRADA
// ============================================================

// Imprime una línea bonita para separar secciones en la consola
void separador() {
    std::cout << "\n================================================\n";
}

// Limpia el flujo de entrada de cin para evitar que los '\n'
// o datos basura nos salten los próximos cin >> o getline
void limpiarBuffer() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// ============================================================
//  FLUJO PRINCIPAL DE LA PARTIDA
// ============================================================

int jugarPartida(const std::string& nombreDetective, ScoreRecord*& raizABB) {

    // 1. Instanciamos los objetos core del juego
    Mapa      mapa;       // El grafo o matriz de locaciones
    TablaHash tabla;      // Aquí guardamos los sospechosos (búsqueda rápida O(1))
    Detective detective(nombreDetective);

    // 2. Ubicamos al jugador en un punto aleatorio del mapa para empezar
    Location* posInicial = mapa.posicionInicialDetective();
    detective.setPosicion(posInicial);

    // 3. Obtenemos quién es el malo del paseo desde la tabla hash
    std::string nombreCulpable = tabla.getNombreCulpable();

    // 4. Preparamos las pistas que los testigos le van a dar al detective
    {
        // Lista ordenada de las características que queremos revelar
        std::vector<std::string> atributosCulpable = {
            "La estatura del culpable",
            "El color de cabello del culpable",
            "El tono de piel del culpable",
            "La forma de la nariz del culpable",
            "El sexo del culpable"
        };

        // Repartimos las pistas entre los testigos disponibles en el mapa
        auto& testigos = mapa.getTestigos();
        for (size_t i = 0; i < testigos.size(); ++i) {

            // i % size nos asegura que si hay más de 5 testigos,
            // el índice vuelva a 0 y no nos salgamos del vector (evita desbordamiento)
            std::string decl = atributosCulpable[i % atributosCulpable.size()];

            // NOTA: El (void) decl está aquí solo para que el compilador no moleste
            // diciendo que la variable no se usa. La pista real se revelará
            // dinámicamente cuando el jugador use la función 'revelarAtributoCulpable()'.
            (void)decl;
        }
    }

    // Al final del juego, este metodo debe retornar el puntaje
    // para poder insertarlo en el ABB de puntajes (Leaderboard).
}
