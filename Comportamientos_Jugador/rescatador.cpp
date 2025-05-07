#include "../Comportamientos_Jugador/rescatador.hpp"
#include "motorlib/util.h"
#include <iostream>

Action ComportamientoRescatador::think(Sensores sensores)
{
	Action accion = IDLE;

	switch (sensores.nivel)
	{
	case 0:
		accion = ComportamientoRescatadorNivel_0 (sensores);
		break;
	case 1:
		// accion = ComportamientoRescatadorNivel_1 (sensores);
		break;
	case 2:
		// accion = ComportamientoRescatadorNivel_2 (sensores);
		break;
	case 3:
		// accion = ComportamientoRescatadorNivel_3 (sensores);
		break;
	case 4:
		// accion = ComportamientoRescatadorNivel_4 (sensores);
		break;
	}

	return accion;
}

int ComportamientoRescatador::interact(Action accion, int valor)
{

	return 0;
}

char ViablePorAlturaR(char casilla, int dif, bool zap){
	if(abs(dif) <= 1 || (zap && abs(dif) <= 2)){
		return casilla;
	}else{
		return 'P';
	}
}

int ComportamientoRescatador::VeoCasillaInteresante(char i, char c, char d, Sensores sensores){
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
	if(c == 'C' && visitados.find(coordenada2) == visitados.end()) return 2;
	else if(i == 'C' && visitados.find(coordenada1) == visitados.end()) return 1;
	else if(d == 'C' && visitados.find(coordenada3) == visitados.end()) return 3;
	else return 0;
}

void SituarSensorEnMapaR(vector<vector<unsigned char>> &m, vector<vector<unsigned char>> &a, Sensores sensores)
{
    // cout << "estoy en situarsensor en matriz de mapan\n";
    m[sensores.posF][sensores.posC] = sensores.superficie[0];

    int pos = 1;
    switch (sensores.rumbo)
    {
    case norte:
        m[sensores.posF - 1][sensores.posC - 1] = sensores.superficie[1];
        m[sensores.posF - 1][sensores.posC] = sensores.superficie[2];
        m[sensores.posF - 1][sensores.posC + 1] = sensores.superficie[3];
        break;
    case noreste:
        m[sensores.posF - 1][sensores.posC] = sensores.superficie[1];
        m[sensores.posF - 1][sensores.posC + 1] = sensores.superficie[2];
        m[sensores.posF][sensores.posC + 1] = sensores.superficie[3];
        break;
    case este:
        m[sensores.posF - 1][sensores.posC + 1] = sensores.superficie[1];
        m[sensores.posF][sensores.posC + 1] = sensores.superficie[2];
        m[sensores.posF + 1][sensores.posC + 1] = sensores.superficie[3];
        break;
    case sureste:
		m[sensores.posF][sensores.posC + 1] = sensores.superficie[1];
		m[sensores.posF + 1][sensores.posC + 1] = sensores.superficie[2];
		m[sensores.posF + 1][sensores.posC] = sensores.superficie[3];
        break;
    case sur:
		m[sensores.posF + 1][sensores.posC + 1] = sensores.superficie[1];
		m[sensores.posF + 1][sensores.posC] = sensores.superficie[2];
		m[sensores.posF + 1][sensores.posC - 1] = sensores.superficie[3];
        break;
    case suroeste:
		m[sensores.posF + 1][sensores.posC] = sensores.superficie[1];
		m[sensores.posF + 1][sensores.posC - 1] = sensores.superficie[2];
		m[sensores.posF][sensores.posC - 1] = sensores.superficie[3];
		break;
    case oeste:
		m[sensores.posF + 1][sensores.posC - 1] = sensores.superficie[1];
		m[sensores.posF][sensores.posC - 1] = sensores.superficie[2];
		m[sensores.posF - 1 ][sensores.posC - 1] = sensores.superficie[3];
        break;
    case noroeste:
		m[sensores.posF][sensores.posC - 1] = sensores.superficie[1];
		m[sensores.posF - 1][sensores.posC - 1] = sensores.superficie[2];
		m[sensores.posF - 1][sensores.posC] = sensores.superficie[3];
        break;
    }
}

vector<pair<int,int>> ComportamientoRescatador::pos_casilla_alrededor(Sensores sensores){
	switch(sensores.rumbo){
		case norte:
        return {{sensores.posF - 1, sensores.posC - 1}, {sensores.posF - 1, sensores.posC}, {sensores.posF - 1, sensores.posC + 1}};
    case noreste:
        return {{sensores.posF - 1, sensores.posC}, {sensores.posF - 1, sensores.posC + 1}, {sensores.posF, sensores.posC + 1}};
    case este:
        return {{sensores.posF - 1, sensores.posC + 1}, {sensores.posF, sensores.posC + 1}, {sensores.posF + 1, sensores.posC + 1}};
    case sureste:
		return {{sensores.posF, sensores.posC + 1}, {sensores.posF + 1, sensores.posC + 1}, {sensores.posF + 1, sensores.posC}};
    case sur:
		return {{sensores.posF + 1, sensores.posC + 1}, {sensores.posF + 1, sensores.posC}, {sensores.posF + 1, sensores.posC - 1}};
    case suroeste:
		return {{sensores.posF + 1, sensores.posC}, {sensores.posF + 1, sensores.posC - 1}, {sensores.posF, sensores.posC - 1}};
    case oeste:
		return {{sensores.posF + 1, sensores.posC - 1}, {sensores.posF, sensores.posC - 1}, {sensores.posF - 1 , sensores.posC - 1}};
    case noroeste:
		return {{sensores.posF, sensores.posC - 1}, {sensores.posF - 1, sensores.posC - 1}, {sensores.posF - 1, sensores.posC}};
	}
}

//Si se deben hacer n turns 45 left, se devuelve un numero negativo
int ComportamientoRescatador::ahorro_giro(){
	int giros_sr = 0;
	while(acciones[backtrack_it] != WALK){
		if(acciones[backtrack_it] == TURN_SR) giros_sr++;
		else if(acciones[backtrack_it] == TURN_L) giros_sr -= 2;
		backtrack_it--;
	}
	return giros_sr % 8;
}

Action ComportamientoRescatador::ComportamientoRescatadorNivel_0(Sensores sensores)
{
	Action accion;
	visitados.emplace(sensores.posF, sensores.posC);

	// El comportamiento de seguir un camino hasta encontrar un puesto base.
	SituarSensorEnMapaR(mapaResultado, mapaCotas, sensores);
	if(sensores.superficie[0] == 'D') tiene_zapatillas = true;
	//Comportamiento
	//Estado inicial: una casilla cualquiera dentro del mapa
	//Estado objetivo: estar sobre una casilla 'X'
	if(sensores.superficie[0] == 'X'){					//He llegado al puesto base
		accion = IDLE;									
	}else if(giro45izq > 0){
		accion = TURN_SR;
		giro45izq--;
		acciones.push_back(accion);
		cout << " SR " << endl;
	}else if(giro90izq > 0){
		accion = TURN_L;
		acciones.push_back(accion);
		giro90izq--;
		cout << " LL " << endl;
	}else if(backtrack == true){
		char i = ViablePorAlturaR(sensores.superficie[1], sensores.cota[1] - sensores.cota[0], tiene_zapatillas);
		char c = ViablePorAlturaR(sensores.superficie[2], sensores.cota[2] - sensores.cota[0], tiene_zapatillas);
		char d = ViablePorAlturaR(sensores.superficie[3], sensores.cota[3] - sensores.cota[0], tiene_zapatillas);
		
		int pos = VeoCasillaInteresante(i, c, d, sensores);
		if(sensores.agentes[2] == 'a' && acciones[backtrack_it] == WALK && pos == 0){
			accion = IDLE;
			cout << " OL" << endl;
		}else if(pos == 0 && contador0 == 3){
			contador0 = 0;
			if(acciones[backtrack_it] == WALK){
				accion = WALK;
				backtrack_it--;
			}else if(acciones[backtrack_it] != WALK){
				int giros_sr = ahorro_giro();
				if(giros_sr < 4){
					accion = TURN_SR;
					giro45izq = giros_sr - 1;
				}else{
					accion = TURN_L;
					giro90izq = ((8 - giros_sr) / 2) - 1;
					if(giros_sr % 2 != 0){
						giro45izq = 1;
					}

				}
			}
			//WALK = 0
			//TURN_L = 2
			//TURN_SR = 3
			cout << acciones[backtrack_it] << " accion backtrack" << endl;
			acciones.push_back(accion);
		}else if(pos != 0){
			cout << "Adios back" << endl;
			contador0 = 0;
			backtrack = false;
			switch(pos){
				case 2:
					contador0 = 0;
					accion = WALK;
					break;
				case 1:
					giro45izq = 1;
					accion = TURN_L;
					break;
				case 3:
					accion = TURN_SR;
					break;
			}
			acciones.push_back(accion);
		}else{
			cout << "NUEVO" << endl;
			contador0++;
			if(contador0 == 2){			//Segunda vez que nos encontramos con nada de forma seguida 
				accion = TURN_SR;
				giro45izq = 3;			//Giramos 90 grados a derecha, antes que ir para atras con el turn_l  otra vez
			}else if(contador0 == 3){	//No hay na a la derecha, volvemos al rumbo del backtrack
				accion = TURN_L;
			}else if(contador0 == 1){
				accion = TURN_L;
			}
		}
	}else{
		char i = ViablePorAlturaR(sensores.superficie[1], sensores.cota[1] - sensores.cota[0], tiene_zapatillas);
		char c = ViablePorAlturaR(sensores.superficie[2], sensores.cota[2] - sensores.cota[0], tiene_zapatillas);
		char d = ViablePorAlturaR(sensores.superficie[3], sensores.cota[3] - sensores.cota[0], tiene_zapatillas);
		
		//Chocan agentes
		if(sensores.agentes[2] == 'a'){
			c = 'P';
		}	
		int pos = VeoCasillaInteresante(i, c, d, sensores);
		
			switch(pos){
				case 2:
					contador0 = 0;
					accion = WALK;
					break;
				case 1:
					giro45izq = 1;
					accion = TURN_L;
					break;
				case 3:
					accion = TURN_SR;
					break;
				case 0:
					contador0++;
					if(contador0 == 2){			//Segunda vez que nos encontramos con nada de forma seguida 
						accion = TURN_SR;
						giro45izq = 3;			//Giramos 90 grados a derecha, antes que ir para atras con el turn_l  otra vez
					}else if(contador0 == 3){	//No hay na a la derecha, miramos atras
						accion = TURN_SR;
						giro45izq = 1;
					}else if(contador0 == 4){	//Aqui ya hemos explorado deecha, izquierda, y al dar la vuelta no encontramos una casilla que no haya sido visitado
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
					}else{	
						accion = TURN_L;					
					}
					break;
			}
			
			acciones.push_back(accion);
	}
	//Devolver la accion, actualizar lo que era antes camino a ahora ser la casilla anterior, que estara marcada como precipicio
	ultima_accion = accion;
	return accion;
}

Action ComportamientoRescatador::ComportamientoRescatadorNivel_1(Sensores sensores)
{
}

Action ComportamientoRescatador::ComportamientoRescatadorNivel_2(Sensores sensores)
{
}

Action ComportamientoRescatador::ComportamientoRescatadorNivel_3(Sensores sensores)
{
}

Action ComportamientoRescatador::ComportamientoRescatadorNivel_4(Sensores sensores)
{
}
