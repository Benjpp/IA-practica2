#include "../Comportamientos_Jugador/auxiliar.hpp"
#include <iostream>
#include "motorlib/util.h"

Action ComportamientoAuxiliar::think(Sensores sensores)
{
	Action accion = IDLE;

	switch (sensores.nivel)
	{
	case 0:
		accion = ComportamientoAuxiliarNivel_0 (sensores);
		break;
	case 1:
		// accion = ComportamientoAuxiliarNivel_1 (sensores);
		break;
	case 2:
		// accion = ComportamientoAuxiliarNivel_2 (sensores);
		break;
	case 3:
		// accion = ComportamientoAuxiliarNivel_3 (sensores);
		break;
	case 4:
		// accion = ComportamientoAuxiliarNivel_4 (sensores);
		break;
	}

	return accion;
}

char ViablePorAlturaA(char casilla, int dif){
	if(abs(dif) <= 1){
		return casilla;
	}else{
		return 'P';
	}
}

int VeoCasillaInteresanteA(char i, char c, char d, bool zap){
	//Si veo el puesto base, lo devuelvo
	if(c == 'X') return 2;
	else if(i == 'X') return 1;
	else if(d == 'X') return 3;
	else if(!zap){
		if(c == 'D') return 2;
		else if(i == 'D') return 1;
		else if(d == 'D') return 3;
	}
	if(c == 'C') return 2;
	else if(i == 'C') return 1;
	else if(d == 'C') return 3;
	if(zap){
		if(c == 'B') return 2;
		else if(i == 'B') return 1;
		else if(d == 'B') return 3;
	}
	return 0;	
}

int ComportamientoAuxiliar::interact(Action accion, int valor)
{
	return 0;
}

Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_0(Sensores sensores)
{
	Action accion;
	// El comportamiento de seguir un camino hasta encontrar un puesto base.
	if(sensores.superficie[0] == 'D') tiene_zapatillas = true;

	//Comportamiento
	if(sensores.superficie[0] == 'X'){					//He llegado al puesto base
		accion = IDLE;
	}else if(giro45izq != 0){
		accion = TURN_SR;
		giro45izq--;
	}else{ 
		char i = ViablePorAlturaA(sensores.superficie[1], sensores.cota[1] - sensores.cota[0]); 
		char c = ViablePorAlturaA(sensores.superficie[2], sensores.cota[2] - sensores.cota[0]);
		char d = ViablePorAlturaA(sensores.superficie[3], sensores.cota[3] - sensores.cota[0]);

		int pos = VeoCasillaInteresanteA(i, c, d, tiene_zapatillas);

		switch(pos){
			case 2:
				accion = WALK;
				break;
			case 1:
				giro45izq = 6;
				accion = TURN_SR;
				break;
			case 3: 
				accion = TURN_SR;
				break;				
			case 0:
				giro45izq = 6;
				accion = TURN_SR; 
		}
	}

	//Devolver accion
	ultima_accion = accion;
	return accion;
}

Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_1(Sensores sensores)
{
}

Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_2(Sensores sensores)
{
}

Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_3(Sensores sensores)
{
}

Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_4(Sensores sensores)
{
}