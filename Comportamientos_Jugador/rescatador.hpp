#ifndef COMPORTAMIENTORESCATADOR_H
#define COMPORTAMIENTORESCATADOR_H

#include <list>
#include <set>
#include <chrono>
#include <time.h>
#include <thread>
#include <queue>
#include "comportamientos/comportamiento.hpp"

typedef struct EstadoR{
  int f;
  int c;
  int rumbo;
  bool zapatillas;

  bool operator==(const EstadoR &estado) const{
    return f == estado.f && c == estado.c && rumbo == estado.rumbo && zapatillas == estado.zapatillas;
  }
}EstadoR;

typedef struct NodoR{
  EstadoR estado;
  list<Action> secuencia;
  int coste;

  bool operator==(const NodoR &node) const{
    return estado == node.estado;
  }
  bool operator<(const NodoR &node) const{
    if (estado.f < node.estado.f) return true;
    else if (estado.f == node.estado.f && estado.c < node.estado.c) return true;
    else if (estado.f == node.estado.f && estado.c == node.estado.c && estado.rumbo < node.estado.rumbo) return true;
    else if (estado.f == node.estado.f && estado.c == node.estado.c && estado.rumbo ==
      node.estado.rumbo && estado.zapatillas < node.estado.zapatillas) return true;
    else return false;
 }

}NodoR;

class ComportamientoRescatador : public Comportamiento
{

public:
  ComportamientoRescatador(unsigned int size = 0) : Comportamiento(size)
  {
    //Variables estado nivel 0, 1, 4
    contador0 = 0;
    ultima_accion = IDLE;
    tiene_zapatillas = false;
    giro45izq = 0;
    giro90izq = 0;
    backtrack = false; 
    visitadosR = {};
    acciones = {};
  }
  ComportamientoRescatador(std::vector<std::vector<unsigned char>> mapaR, std::vector<std::vector<unsigned char>> mapaC) : Comportamiento(mapaR,mapaC)
  {
    //Variables estado nivel 2, 3
    hay_plan = false;
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
  //Vars de nivel 0 y 1
  Action ultima_accion;		//Guarda la ultima accion realizada
  bool tiene_zapatillas;	//True si tiene zapatos, falso en otro  caso
  int giro45izq;		//Usado para realizar giros
  int giro90izq;
  int contador0;
  bool backtrack;
  set<pair<int, int>> visitadosR;
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

  //Funciones nivel 2
  int CosteEnergia(const EstadoR& st, Action accion, const vector<vector<unsigned char>>& terreno, const vector<vector<unsigned char>>& altura);
  bool Find(const NodoR& st, const list<NodoR>& lista);
  void VisualizaPlan(const EstadoR& st, const list<Action>& plan);
  list<Action> AnchuraRescatador(const EstadoR& inicio, const EstadoR& final, const vector<vector<unsigned char>>& terreno, const vector<vector<unsigned char>>& altura, const Sensores& sensores);
};

#endif