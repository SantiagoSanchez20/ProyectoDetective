#include <iostream>


#include <string>
#include <limits>

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



// Imprime una línea bonita para separar secciones en la consola
void separador() {
    std::cout << "\n================================================\n";
}

// Limpia el flujo de entrada de cin para evitar que los '\n'
// o datos basura nos salten los próximos cin >> o getline
void limpiarBuffer() {
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
}

// flujo principal de la partida

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


    // Pantalla de bienvenida
    separador();
    std::cout << "  CASO ABIERTO — Bienvenido, detective " << nombreDetective << "!\n";
    std::cout << "  Recolecta las 10 pistas y acusa al culpable.\n";
    separador();
    std::cout << "\n  CONTROLES:\n"
              << "  W/A/S/D  -> mover (arriba/izq/abajo/der)\n"
              << "  X        -> usar la ultima pista de la pila\n"
              << "  T        -> ver pila de pistas recogidas\n"
              << "  S        -> ver tabla de sospechosos\n"
              << "  I        -> interrogar testigo de la cola\n"
              << "  R        -> ver ranking historico\n"
              << "  Q        -> abandonar partida\n\n";

    mapa.imprimir(detective.getPosicion());

    // Loop principal del juego
    bool partidaActiva = true;
    bool gano          = false;

    while (partidaActiva) {

        std::cout << nombreDetective
                  << ", Tu puntaje actual es: "
                  << detective.getPuntaje()
                  << "  |  Pistas recogidas: "
                  << detective.getPistasRecogidas()
                  << "/10\n";
        std::cout << "Accion > ";

        char cmd;
        std::cin >> cmd;
        cmd = static_cast<char>(toupper(static_cast<unsigned char>(cmd)));

        // Movimiento
        if (cmd == 'W' || cmd == 'A' || cmd == 'S' || cmd == 'D') {

            Location* actual  = detective.getPosicion();
            Location* destino = nullptr;

            if      (cmd == 'W') destino = actual->arriba;
            else if (cmd == 'S') destino = actual->abajo;
            else if (cmd == 'A') destino = actual->izquierda;
            else if (cmd == 'D') destino = actual->derecha;

            // Borde: no hay nodo en esa dirección
            if (destino == nullptr) {
                std::cout << "  [!] No puedes salir de la ciudad.\n";
                detective.mover(); // igual suma 1 punto por intento

                // Edificio en el borde exterior
            } else if (destino->getTipo() == TipoUbicacion::EDIFICIO) {
                std::cout << "  [!] Hay un edificio. No puedes pasar.\n";
                detective.mover();

                // Callejón cerrado: se hace visible pero no se mueve
            } else if (destino->getTipo() == TipoUbicacion::CALLEJON_CERRADO) {
                std::cout << "  [!] Callejon cerrado '|'. Busca otra ruta.\n";
                destino->setDescubierta(true);   // se revela en el tablero
                detective.mover();

                // Movimiento válido
            } else {
                detective.mover();
                detective.setPosicion(destino);

                // Marcar como descubierta y calle abierta si era NO_VISITADA
                if (!destino->isDescubierta()) {
                    destino->setDescubierta(true);

                    TipoUbicacion t = destino->getTipo();
                    if (t == TipoUbicacion::NO_VISITADA) {
                        destino->setTipo(TipoUbicacion::CALLE_ABIERTA);
                    }
                }

                // ¿Hay pista aquí?
                if (destino->getPista() != nullptr &&
                    !destino->getPista()->isUsada()) {

                    Pista* p = destino->getPista();
                    p->setUsada(true);

                    // Recoger pista
                    detective.recogerPista(p);

                    // Revelar atributo del culpable
                    std::string atributo = tabla.revelarAtributoCulpable();

                    std::cout << "\n  *** PISTA ENCONTRADA: ["
                              << p->getSimbolo() << "] "
                              << p->getNombreTipo() << " ***\n";
                    if (!atributo.empty() && atributo != "No hay mas atributos por revelar.") {
                        std::cout << "  Dato revelado del culpable: "
                                  << atributo << "\n";
                    }
                    std::cout << "  Pistas recogidas: "
                              << detective.getPistasRecogidas() << "/10\n\n";

                    // ¿Caso completo?
                    if (detective.casoCompleto()) {
                        partidaActiva = false; // salimos del loop para acusar
                    }
                    }

                // ¿Hay testigo aquí?
                if (destino->tieneTestigo() && partidaActiva) {
                    // Buscamos el testigo en la lista del mapa
                    auto& testigos = mapa.getTestigos();
                    for (Testigo* t : testigos) {
                        // Añadimos a la cola (puede añadirse varias veces
                        // si el detective vuelve, el enunciado no lo restringe)
                        detective.agregarTestigo(t);
                        std::cout << "  [W] Testigo encontrado: "
                                  << t->getNombre()
                                  << ". Usa 'I' para interrogarlo.\n";
                        break; // solo el primero no procesado
                    }
                }
            }

            mapa.imprimir(detective.getPosicion());

            // Usar pista (X)
        } else if (cmd == 'X') {

            if (!detective.tienePistas()) {
                std::cout << "  No tienes pistas en la pila.\n";
            } else {
                Pista* p = detective.usarPista();
                p->setUsada(false); // volverá al mapa

                std::cout << "\n  Usas la pista ["
                          << p->getSimbolo() << "] "
                          << p->getNombreTipo() << ".\n";

                switch (p->getTipo()) {

                    case TipoPista::HUELLA:
                        detective.reducirPuntajeAMitad();
                        std::cout << "  [H] Puntaje reducido a la mitad: "
                                  << detective.getPuntaje() << "\n";
                        break;

                    case TipoPista::COARTADA:
                        mapa.eliminarDosCallejones();
                        std::cout << "  [C] Dos callejones eliminados del mapa.\n";
                        break;

                    case TipoPista::TESTIMONIO: {
                        int dado = rand() % 2;
                        if (dado == 0) {
                            detective.reducirPuntajeACero();
                            std::cout << "  [T] Suerte! Puntaje reducido a cero.\n";
                        } else {
                            detective.duplicarPuntaje();
                            std::cout << "  [T] Mala suerte. Puntaje duplicado a: "
                                      << detective.getPuntaje() << "\n";
                        }
                        break;
                    }

                    case TipoPista::PRUEBA_FORENSE: {
                        // Teletransporte a posición no descubierta
                        // (usamos nodoParaTeletransporte via acceso a grilla)
                        // Como nodoParaTeletransporte es privado, lo simulamos
                        // buscando un nodo libre con posicionInicialDetective
                        // alternativa: buscamos en el mapa directamente
                        Location* nuevo = nullptr;
                        // Intento hasta 200 veces para encontrar nodo válido
                        for (int intentos = 0; intentos < 200 && nuevo == nullptr; ++intentos) {
                            int f = 1 + rand() % (FILAS - 2);
                            int c = 1 + rand() % (COLUMNAS - 2);
                            Location* cand = mapa.getNodo(f, c);
                            if (cand != nullptr &&
                                !cand->isDescubierta() &&
                                cand->getTipo() != TipoUbicacion::CALLEJON_CERRADO &&
                                cand->getTipo() != TipoUbicacion::EDIFICIO) {
                                nuevo = cand;
                                }
                        }
                        if (nuevo != nullptr) {
                            detective.setPosicion(nuevo);
                            nuevo->setDescubierta(true);
                            nuevo->setTipo(TipoUbicacion::CALLE_ABIERTA);
                            std::cout << "  [P] Teletransportado a ("
                                      << nuevo->getFila() << ","
                                      << nuevo->getColumna() << ").\n";
                        } else {
                            std::cout << "  [P] No se encontro posicion libre. Sin efecto.\n";
                        }
                        break;
                    }
                }

                // La pista vuelve al mapa en posición aleatoria
                mapa.cubrirMapa();
                mapa.reubcarPista(p);
                std::cout << "  La pista ["
                          << p->getSimbolo()
                          << "] fue reubicada. El mapa se ha cubierto.\n\n";

                mapa.imprimir(detective.getPosicion());
            }

            //  Ver pila de pistas (T)
        } else if (cmd == 'T') {
            detective.mostrarPilas();

            // Ver tabla de sospechosos (S)
        } else if (cmd == 'S') {
            tabla.mostrarRevelados();

            // Interrogar testigo de la cola (I)
        } else if (cmd == 'I') {
            if (!detective.tieneTestigos()) {
                std::cout << "  No tienes testigos en la cola.\n";
            } else {
                Testigo* t = detective.interrogarTestigo();
                std::string atributo = tabla.revelarAtributoCulpable();

                std::cout << "\n  [Testigo] " << t->getNombre()
                          << " declara:\n";
                if (!atributo.empty() && atributo != "No hay mas atributos por revelar.") {
                    std::cout << "  >> \"El culpable tiene: " << atributo << "\"\n\n";
                } else {
                    std::cout << "  >> \"No tengo mas informacion que agregar.\"\n\n";
                }
            }

            // Ver ranking (R)
        } else if (cmd == 'R') {
            abbMostrarRanking(raizABB);

            // Abandonar (Q)
        } else if (cmd == 'Q') {
            std::cout << "  Abandonaste la partida.\n";
            detective.penalizarPuntaje(); // puntaje se duplica (req. 18 fracaso)
            partidaActiva = false;
            gano = false;

        } else {
            std::cout << "  Comando no reconocido. Usa W/A/S/D, X, T, S, I, R o Q.\n";
        }
    } // fin while

    if (detective.casoCompleto()) {
        separador();
        std::cout << "\n  " << nombreDetective
                  << ", has recolectado las 10 pistas. Es momento de acusar.\n\n";

        // Mostrar tabla completa con lo revelado hasta ahora
        tabla.mostrarCompleta();

        // Listar nombres de sospechosos
        std::vector<std::string> nombres = tabla.getNombres();
        std::cout << "  Sospechosos disponibles: ";
        for (size_t i = 0; i < nombres.size(); ++i) {
            std::cout << nombres[i];
            if (i + 1 < nombres.size()) std::cout << ", ";
        }
        std::cout << "\n\n";

        std::cout << "  A quien acusas? > ";
        limpiarBuffer();
        std::string acusado;
        std::getline(std::cin, acusado);

        // Búsqueda O(1) en la tabla hash — explícita para sustentación
        std::cout << "\n  [TablaHash] Buscando \"" << acusado
                  << "\" en la tabla hash (O(1) promedio)...\n";
        Sospechoso* resultado = tabla.buscar(acusado);

        if (resultado == nullptr) {
            std::cout << "  Ese nombre no esta en el caso. Acusacion invalida.\n";
            detective.penalizarPuntaje();
            gano = false;
        } else if (resultado->isCulpable()) {
            std::cout << "\n  *** CASO RESUELTO! ***\n";
            std::cout << "  " << acusado << " era el culpable.\n";
            std::cout << "  Puntaje final: " << detective.getPuntaje()
                      << " movimientos.\n";
            gano = true;
        } else {
            std::cout << "\n  Acusacion incorrecta. "
                      << acusado << " no es el culpable.\n";
            std::cout << "  El verdadero culpable era: "
                      << tabla.getNombreCulpable() << ".\n";
            detective.penalizarPuntaje(); // puntaje se duplica
            std::cout << "  Puntaje penalizado: " << detective.getPuntaje()
                      << " movimientos.\n";
            gano = false;
        }
    }

    separador();
    if (gano) {
        std::cout << "  VICTORIA! Puntaje final: "
                  << detective.getPuntaje() << " movimientos.\n";
    } else {
        std::cout << "  DERROTA. Puntaje final: "
                  << detective.getPuntaje() << " movimientos.\n";
    }
    separador();

    return detective.getPuntaje();
}


void menuPrincipal() {

    std::cout << "\n+==========================================+\n";
    std::cout << "|        EL CASO DEL DETECTIVE             |\n";
    std::cout << "|   Proyecto Final -- Estructuras de Datos |\n";
    std::cout << "+==========================================+\n\n";

    std::cout << "  1. Nueva partida\n";
    std::cout << "  2. Buscar puntaje de un detective\n";
    std::cout << "  3. Ver ranking historico\n";
    std::cout << "  4. Salir\n\n";
    std::cout << "Opcion > ";
}


int main() {
    srand(static_cast<unsigned int>(time(nullptr)));

    ScoreRecord* raizABB = nullptr; // ABB de puntajes históricos
    bool salir = false;

    while (!salir) {
        menuPrincipal();

        int opcion;
        std::cin >> opcion;

        switch (opcion) {

            // Nueva partida
            case 1: {
                std::cout << "\n  Ingresa tu nombre, detective: ";
                limpiarBuffer();
                std::string nombre;
                std::getline(std::cin, nombre);

                if (nombre.empty()) {
                    std::cout << "  Nombre invalido.\n";
                    break;
                }

                // Verificar si ya jugó antes (req. 22)
                ScoreRecord* previo = abbBuscar(raizABB, nombre);
                if (previo != nullptr) {
                    std::cout << "  " << nombre
                              << " ya ha jugado. Mejor puntaje previo: "
                              << previo->getMejorPuntaje()
                              << " movimientos.\n\n";
                }

                // Jugar
                int puntajeFinal = jugarPartida(nombre, raizABB);

                // Guardar en el ABB (req. 19-21)
                raizABB = abbInsertar(raizABB, nombre, puntajeFinal);
                std::cout << "  Puntaje guardado en el ABB.\n";
                break;
            }

            // Buscar detective requerimiento 22
            case 2: {
                std::cout << "\n  Nombre del detective a buscar: ";
                limpiarBuffer();
                std::string nombre;
                std::getline(std::cin, nombre);

                ScoreRecord* rec = abbBuscar(raizABB, nombre);
                if (rec == nullptr) {
                    std::cout << "  " << nombre
                              << " no tiene partidas registradas.\n";
                } else {
                    std::cout << "  " << rec->getNombreDetective()
                              << " | Mejor puntaje: "
                              << rec->getMejorPuntaje()
                              << " movimientos.\n";
                }
                break;
            }

            // Ranking requerimiento 23
            case 3:
                abbMostrarRanking(raizABB);
                break;

            // Salir
            case 4:
                salir = true;
                std::cout << "\n  Hasta pronto, detective.\n\n";
                break;

            default:
                std::cout << "  Opcion invalida.\n";
                break;
        }
    }

    // Liberar memoria del ABB al cerrar
    abbLiberar(raizABB);

    return 0;
}