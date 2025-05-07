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

int ComportamientoAuxiliar::VeoCasillaInteresante(char i, char c, char d, Sensores sensores){
	if(c == 'X') return 2;
	else if(i == 'X') return 1;
	else if(d == 'X') return 3; 
	if(!tiene_zapatillas){
		if(c == 'D') return 2;
		else if(i == 'D') return 1;
		else if(d == 'D') return 3;
	}
	//Damos prioridad a las casillas por las que no hemos pasado
	vector<pair<int, int>> alrededor = pos_casilla_alrededor(sensores);
	pair<int, int> coordenada1 = alrededor[0]; 
	pair<int, int> coordenada2 = alrededor[1]; 
	pair<int, int> coordenada3 = alrededor[2]; 
	//Priorizamos los que no estan en el conjunto visitados
	if(c == 'C' && visitadosA.find(coordenada2) == visitadosA.end()) return 2;
	else if(i == 'C' && visitadosA.find(coordenada1) == visitadosA.end()) return 1;
	else if(d == 'C' && visitadosA.find(coordenada3) == visitadosA.end()) return 3;
	else return 0;
}

int ComportamientoAuxiliar::interact(Action accion, int valor)
{
	return 0;
}

vector<pair<int,int>> ComportamientoAuxiliar::pos_casilla_alrededor(Sensores sensores){
	switch(sensores.rumbo){
		case norte:
        return {{sensores.posF - 1, sensores.posC - 1}, {sensores.posF - 1, sensores.posC}, {sensores.posF - 1, sensores.posC + 1}};
		break;
    case noreste:
        return {{sensores.posF - 1, sensores.posC}, {sensores.posF - 1, sensores.posC + 1}, {sensores.posF, sensores.posC + 1}};
		break;
	case este:
        return {{sensores.posF - 1, sensores.posC + 1}, {sensores.posF, sensores.posC + 1}, {sensores.posF + 1, sensores.posC + 1}};
		break;
	case sureste:
		return {{sensores.posF, sensores.posC + 1}, {sensores.posF + 1, sensores.posC + 1}, {sensores.posF + 1, sensores.posC}};
		break;
	case sur:
		return {{sensores.posF + 1, sensores.posC + 1}, {sensores.posF + 1, sensores.posC}, {sensores.posF + 1, sensores.posC - 1}};
		break;
	case suroeste:
		return {{sensores.posF + 1, sensores.posC}, {sensores.posF + 1, sensores.posC - 1}, {sensores.posF, sensores.posC - 1}};
		break;
	case oeste:
		return {{sensores.posF + 1, sensores.posC - 1}, {sensores.posF, sensores.posC - 1}, {sensores.posF - 1 , sensores.posC - 1}};
		break;
	case noroeste:
		return {{sensores.posF, sensores.posC - 1}, {sensores.posF - 1, sensores.posC - 1}, {sensores.posF - 1, sensores.posC}};
		break;
	}
}

int ComportamientoAuxiliar::ahorro_giro(){
	int giros_sr = 0;
	while(acciones[backtrack_it] == TURN_SR){
		backtrack_it--;
		giros_sr++;
	}
	giros_sr = giros_sr % 8;
	return giros_sr;
}

Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_0(Sensores sensores)
{
	Action accion;
	visitadosA.emplace(sensores.posF, sensores.posC);

	// El comportamiento de seguir un camino hasta encontrar un puesto base.
	if(sensores.superficie[0] == 'D') tiene_zapatillas = true;

	//Comportamiento
	if(sensores.superficie[0] == 'X'){					//He llegado al puesto base
		accion = IDLE;
	}else if(giro45izq > 0){
		accion = TURN_SR;
		giro45izq--;
		acciones.push_back(accion);
	}else if(backtrack == true){
		char i = ViablePorAlturaA(sensores.superficie[1], sensores.cota[1] - sensores.cota[0]);
		char c = ViablePorAlturaA(sensores.superficie[2], sensores.cota[2] - sensores.cota[0]);
		char d = ViablePorAlturaA(sensores.superficie[3], sensores.cota[3] - sensores.cota[0]);
		
		int pos = VeoCasillaInteresante(i, c, d, sensores);
		if(sensores.agentes[2] == 'r' && acciones[backtrack_it] == WALK && pos == 0){
			accion = IDLE;
		}else if(pos == 0 && contador0 == 3){
			contador0 = 0;
			if(acciones[backtrack_it] == WALK){
				accion = WALK;
				backtrack_it--;
			}else if(acciones[backtrack_it] == TURN_SR){
				giro45izq = (8 - ahorro_giro()) - 1;
				accion = TURN_SR;
			}
			acciones.push_back(accion);
		}else if(pos != 0){
			contador0 = 0;
			backtrack = false;
			switch(pos){
				case 2:
					contador0 = 0;
					accion = WALK;
					break;
				case 1:
					giro45izq = 5;
					accion = TURN_SR;
					break;
				case 3:
					accion = TURN_SR;
					break;
			}
			acciones.push_back(accion);
		}else{
			contador0++;
			if(contador0 == 2){			//Segunda vez que nos encontramos con nada de forma seguida, miramos a la izquierda del rumbo original
				accion = TURN_SR;
				giro45izq = 3;			//Giramos 90 grados a derecha, antes que ir para atras con el turn_l  otra vez
			}else if(contador0 == 3){	//No hay na a la derecha, volvemos al rumbo del backtrack
				accion = TURN_SR;
				giro45izq = 1;
			}else if(contador0 == 1){
				giro45izq = 1;			//En la primera miramos a la derecha		
				accion = TURN_SR;
			}
		}
	}else{ 
		char i = ViablePorAlturaA(sensores.superficie[1], sensores.cota[1] - sensores.cota[0]);
		char c = ViablePorAlturaA(sensores.superficie[2], sensores.cota[2] - sensores.cota[0]);
		char d = ViablePorAlturaA(sensores.superficie[3], sensores.cota[3] - sensores.cota[0]);
		
		//Chocan agentes
		if(sensores.agentes[2] == 'r'){
			c = 'P';
		}	
		int pos = VeoCasillaInteresante(i, c, d, sensores);

			switch(pos){
				case 2:
					contador0 = 0;
					accion = WALK;
					break;
				case 1:
					giro45izq = 5;
					accion = TURN_SR;
					break;
				case 3:
					accion = TURN_SR;
					break;
				case 0:
					if(contador0 == 0){
						//Miramos izquierda primera
						accion = TURN_SR;
						giro45izq = 5;		
						contador0++;
					}else if(contador0 == 1){			//Segunda vez que nos encontramos con nada de forma seguida, miramos derecha del rumbo og
						accion = TURN_SR;
						giro45izq = 3;	
						contador0++;
					}else if(contador0 == 2){	//No hay na a la derecha, miramos atras
						accion = TURN_SR;
						giro45izq = 1;
						contador0++;
					}else if(contador0 == 3){	//Aqui ya hemos explorado deecha, izquierda, y al dar la vuelta no encontramos una casilla que no haya sido visitado
						//Comencamos backtraacking hasta encontrar algo "interesante"
						backtrack = true;
						contador0 = 0;		
						backtrack_it = acciones.size() - 1;
						//Dejamos el iterador con una accion del tipo WALK
						while(acciones[backtrack_it] != WALK){
							backtrack_it--;
						}
						//Hacemos el primer walk para no desperdiciar este instante
						accion = acciones[backtrack_it];
						backtrack_it--;
					}
					break;
			}
			acciones.push_back(accion);
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
