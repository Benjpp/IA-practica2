#ifndef COMPORTAMIENTOAUXILIAR_H
#define COMPORTAMIENTOAUXILIAR_H

#include <set>
#include <chrono>
#include <time.h>
#include <thread>

#include "comportamientos/comportamiento.hpp"

class ComportamientoAuxiliar : public Comportamiento
{

public:
  ComportamientoAuxiliar(unsigned int size = 0) : Comportamiento(size)
  {
    contador0 = 0;
    ultima_accion = IDLE;
    tiene_zapatillas = false;
    giro45izq = 0;
    backtrack = false; 
    visitadosA = {};
    acciones = {};
  }
  ComportamientoAuxiliar(std::vector<std::vector<unsigned char>> mapaR, std::vector<std::vector<unsigned char>> mapaC) : Comportamiento(mapaR,mapaC)
  {
    // Inicializar Variables de Estado Niveles 2,3
  }
  ComportamientoAuxiliar(const ComportamientoAuxiliar &comport) : Comportamiento(comport) {}
  ~ComportamientoAuxiliar() {}

  Action think(Sensores sensores);

  int interact(Action accion, int valor);

  Action ComportamientoAuxiliarNivel_0(Sensores sensores);
  Action ComportamientoAuxiliarNivel_1(Sensores sensores);
  Action ComportamientoAuxiliarNivel_2(Sensores sensores);
  Action ComportamientoAuxiliarNivel_3(Sensores sensores);
  Action ComportamientoAuxiliarNivel_4(Sensores sensores);

private:
  Action ultima_accion;
  int giro45izq;
  bool tiene_zapatillas;
  int contador0;
  bool backtrack;
  set<pair<int, int>> visitadosA;
  vector<Action> acciones;
  int backtrack_it;


  int ahorro_giro();
  int VeoCasillaInteresante(char i, char c, char d, Sensores sensores);
  vector<pair<int, int>> pos_casilla_alrededor(Sensores sensores);
};

#endif
