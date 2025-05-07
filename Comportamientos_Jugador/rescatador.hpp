#ifndef COMPORTAMIENTORESCATADOR_H
#define COMPORTAMIENTORESCATADOR_H

#include <list>
#include <set>
#include <chrono>
#include <time.h>
#include <thread>
#include <queue>
#include "comportamientos/comportamiento.hpp"

class ComportamientoRescatador : public Comportamiento
{

public:
  ComportamientoRescatador(unsigned int size = 0) : Comportamiento(size)
  {
    contador0 = 0;
    ultima_accion = IDLE;
    tiene_zapatillas = false;
    giro45izq = 0;
    giro90izq = 0;
    backtrack = false; 
    visitados = {};
    acciones = {};
  }
  ComportamientoRescatador(std::vector<std::vector<unsigned char>> mapaR, std::vector<std::vector<unsigned char>> mapaC) : Comportamiento(mapaR,mapaC)
  {
    // Inicializar Variables de Estado Niveles 2,3
  }
  ComportamientoRescatador(const ComportamientoRescatador &comport) : Comportamiento(comport) {}
  ~ComportamientoRescatador() {}

  Action think(Sensores sensores);

  int interact(Action accion, int valor);

  Action ComportamientoRescatadorNivel_0(Sensores sensores);
  Action ComportamientoRescatadorNivel_1(Sensores sensores);
  Action ComportamientoRescatadorNivel_2(Sensores sensores);
  Action ComportamientoRescatadorNivel_3(Sensores sensores);
  Action ComportamientoRescatadorNivel_4(Sensores sensores);

private:
  Action ultima_accion;		//Guarda la ultima accion realizada
  bool tiene_zapatillas;	//True si tiene zapatos, falso en otro  caso
  int giro45izq;		//Usado para realizar giros
  int giro90izq;
  int contador0;
  bool backtrack;
  set<pair<int, int>> visitados;
  vector<Action> acciones;
  int backtrack_it;

  int ahorro_giro();
  int VeoCasillaInteresante(char i, char c, char d, Sensores sensores);
  vector<pair<int, int>> pos_casilla_alrededor(Sensores sensores);
};

#endif
