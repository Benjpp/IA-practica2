#ifndef COMPORTAMIENTOAUXILIAR_H
#define COMPORTAMIENTOAUXILIAR_H

#include <list>
#include <set>
#include <chrono>
#include <time.h>
#include <thread>

#include "comportamientos/comportamiento.hpp"

typedef struct EstadoA{
  int f;
  int c;
  int rumbo;
  bool zapatillas;

  bool operator==(const EstadoA &estado) const{
    return f == estado.f && c == estado.c && rumbo == estado.rumbo && zapatillas == estado.zapatillas;
  }
}EstadoA;

typedef struct NodoA{
  EstadoA estado;
  list<Action> secuencia;

  bool operator==(const NodoA &node) const{
    return estado == node.estado;
  }
}NodoA;

class ComportamientoAuxiliar : public Comportamiento
{

public:
  ComportamientoAuxiliar(unsigned int size = 0) : Comportamiento(size)
  {
    //Variables estado nivel 0, 1, 4
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
     //Variables estado nivel 2, 3
    hay_plan = false;
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
  //Vars de nivel 0 y 1
  Action ultima_accion;
  int giro45izq;
  bool tiene_zapatillas;
  int contador0;
  bool backtrack;
  set<pair<int, int>> visitadosA;
  vector<Action> acciones;
  int backtrack_it;

  //Vars nivel 2
  bool hay_plan;
  list<Action> plan;

  //Funciones nivel 0 y 1
  int ahorro_giro();
  int VeoCasillaInteresante_nivel0(char i, char c, char d, Sensores sensores);
  int VeoCasillaInteresante_nivel1(char i, char c, char d, Sensores sensores);
  vector<pair<int, int>> pos_casilla_alrededor(Sensores sensores);

  //Funciones nivel 3
  int CosteEnergia(const EstadoA& st, Action accion, const vector<vector<unsigned char>>& terreno, const vector<vector<unsigned char>>& altura);
  list<Action> AnchuraAuxiliar(const EstadoA& inicio, const EstadoA& final, const vector<vector<unsigned char>>& terreno, const vector<vector<unsigned char>>& altura);
  bool Find(const NodoA& st, const list<NodoA>& lista);
  void VisualizaPlan(const EstadoA& st, const list<Action>& plan);
};

#endif
