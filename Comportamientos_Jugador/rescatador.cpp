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
		accion = ComportamientoRescatadorNivel_1 (sensores);
		break;
	case 2:
		accion = ComportamientoRescatadorNivel_2 (sensores);
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

char ViablePorAlturaR(char casilla, int dif, bool zap, Action accion){
	if((abs(dif) <= 1 || (zap && abs(dif) <= 2)) && accion == WALK){
		return casilla;
	}else if((abs(dif) <= 2 || (zap && abs(dif) <= 3)) && accion == RUN){
		return casilla;
	}else{
		return 'P';
	}
}

int ComportamientoRescatador::VeoCasillaInteresante_nivel0(char i, char c, char d, Sensores sensores){
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
	if(c == 'C' && visitadosR.find(coordenada2) == visitadosR.end()) return 2;
	else if(i == 'C' && visitadosR.find(coordenada1) == visitadosR.end()) return 1;
	else if(d == 'C' && visitadosR.find(coordenada3) == visitadosR.end()) return 3;
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
	if(giros_sr > 0) return giros_sr % 8;
	else if(giros_sr < 0) return giros_sr % -8; 
}

Action ComportamientoRescatador::ComportamientoRescatadorNivel_0(Sensores sensores)
{
	Action accion;
	// El comportamiento de seguir un camino hasta encontrar un puesto base.
	SituarSensorEnMapaR(mapaResultado, mapaCotas, sensores);
	visitadosR.emplace(sensores.posF, sensores.posC);
	acciones.push_back(ultima_accion);

	if(sensores.superficie[0] == 'D') tiene_zapatillas = true;
	//Comportamiento
	//Estado inicial: una casilla cualquiera dentro del mapa
	//Estado objetivo: estar sobre una casilla 'X'
	if(sensores.superficie[0] == 'X'){					//He llegado al puesto base
		accion = IDLE;									
	}else if(giro45izq > 0){
		accion = TURN_SR;
		giro45izq--;
		//acciones.push_back(accion);
	}else if(giro90izq > 0){
		accion = TURN_L;
		//acciones.push_back(accion);
		giro90izq--;
	}else if(backtrack == true){
		char i = ViablePorAlturaR(sensores.superficie[1], sensores.cota[1] - sensores.cota[0], tiene_zapatillas, WALK);
		char c = ViablePorAlturaR(sensores.superficie[2], sensores.cota[2] - sensores.cota[0], tiene_zapatillas, WALK);
		char d = ViablePorAlturaR(sensores.superficie[3], sensores.cota[3] - sensores.cota[0], tiene_zapatillas, WALK);
		
		while(acciones[backtrack_it] == IDLE) backtrack_it--;

		if(sensores.agentes[2] == 'a'){
			c == 'P';
		}
		int pos = VeoCasillaInteresante_nivel0(i, c, d, sensores);
		if(pos == 0 && contador0 >= 3){
			contador0 = 0;
			if(acciones[backtrack_it] == WALK){
				if(sensores.agentes[2] != 'a'){
					accion = WALK;
					backtrack_it--;
				}else{
					accion = IDLE;
				}
			}else if(acciones[backtrack_it] != WALK){
				int giros_sr = ahorro_giro();
				//!TODO mirar esto, los if mal hechos
				if(abs(giros_sr) == 4){
					giro90izq = 1;
					accion = TURN_L;
				}else if(giros_sr > 0){
					accion = TURN_L;
					giro90izq = (giros_sr / 2) - 1;
					if(giros_sr % 2 != 0){
						giro45izq = 1;
					}
				}else if(giros_sr < 0){
					accion = TURN_SR;
					giro45izq = abs(giros_sr) - 1;
				}else if(giros_sr == 0){
					accion = WALK;
				}
			}
			//WALK = 0
			//TURN_L = 2
			//TURN_SR = 3
			//acciones.push_back(accion);
		}else if(pos != 0){
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
			//acciones.push_back(accion);
		}else if(contador0 < 3){
			contador0++;
			if(contador0 == 1){
				accion = TURN_L;
			}else if(contador0 == 2){			//Segunda vez que nos encontramos con nada de forma seguida 
				accion = TURN_SR;
				giro45izq = 3;			//Giramos 90 grados a derecha, antes que ir para atras con el turn_l  otra vez
			}else if(contador0 == 3){	//No hay na a la derecha, volvemos al rumbo del backtrack
				accion = TURN_L;
			}
		}
	}else{
		char i = ViablePorAlturaR(sensores.superficie[1], sensores.cota[1] - sensores.cota[0], tiene_zapatillas, WALK);
		char c = ViablePorAlturaR(sensores.superficie[2], sensores.cota[2] - sensores.cota[0], tiene_zapatillas, WALK);
		char d = ViablePorAlturaR(sensores.superficie[3], sensores.cota[3] - sensores.cota[0], tiene_zapatillas, WALK);
		
		//Chocan agentes
		if(sensores.agentes[2] == 'a'){
			c = 'P';
		}	
		int pos = VeoCasillaInteresante_nivel0(i, c, d, sensores);

		switch(pos){
			//En el caso contador 4 tendremos al final de vector TurnL, TURN_sr x4, TurnSrx2
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
				}else if(contador0 == 1){	
					accion = TURN_L;					
				}
				break;
			}
			
			//acciones.push_back(accion);
	}
	//Devolver la accion, actualizar lo que era antes camino a ahora ser la casilla anterior, que estara marcada como precipicio
	ultima_accion = accion;
	return accion;
}

int ComportamientoRescatador::VeoCasillaInteresante_nivel1(char i, char c, char d, Sensores sensores){
	 
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
	if((c == 'C' || c == 'S') && visitadosR.find(coordenada2) == visitadosR.end()) return 2;
	else if((i == 'C' || i == 'S') && visitadosR.find(coordenada1) == visitadosR.end()) return 1;
	else if((d == 'C' || d == 'S') && visitadosR.find(coordenada3) == visitadosR.end()) return 3;
	else return 0;
}

Action ComportamientoRescatador::ComportamientoRescatadorNivel_1(Sensores sensores)
{
	Action accion;
	acciones.push_back(ultima_accion);
	SituarSensorEnMapaR(mapaResultado, mapaCotas, sensores);
	if(sensores.superficie[0] == 'D') tiene_zapatillas = true;

	if(giro45izq > 0){
		accion = TURN_SR;
		giro45izq--;
		//acciones.push_back(accion);
	}else if(giro90izq > 0){
		accion = TURN_L;
		//acciones.push_back(accion);
		giro90izq--;
	}else if(backtrack == true){
		char i = ViablePorAlturaR(sensores.superficie[1], sensores.cota[1] - sensores.cota[0], tiene_zapatillas, WALK);
		char c = ViablePorAlturaR(sensores.superficie[2], sensores.cota[2] - sensores.cota[0], tiene_zapatillas, WALK);
		char d = ViablePorAlturaR(sensores.superficie[3], sensores.cota[3] - sensores.cota[0], tiene_zapatillas, WALK);

		while(acciones[backtrack_it] == IDLE) backtrack_it--;
		
		int pos = VeoCasillaInteresante_nivel1(i, c, d, sensores);
		if(sensores.agentes[2] == 'a' && acciones[backtrack_it] == WALK && pos == 0){
			accion = IDLE;
		}else if(pos == 0 && contador0 == 3){
			contador0 = 0;
			if(acciones[backtrack_it] == WALK){
				if(sensores.agentes[2] != 'a'){
					accion = WALK;
					backtrack_it--;
				}else{
					accion = IDLE;
				}
			}else if(acciones[backtrack_it] != WALK){
				int giros_sr = ahorro_giro();
				//!TODO mirar esto, los if mal hechos
				if(abs(giros_sr) == 4){
					giro90izq = 1;
					accion = TURN_L;
				}else if(giros_sr > 0){
					accion = TURN_L;
					giro90izq = (giros_sr / 2) - 1;
					if(giros_sr % 2 != 0){
						giro45izq = 1;
					}
				}else if(giros_sr < 0){
					accion = TURN_SR;
					giro45izq = abs(giros_sr) - 1;
				}else if(giros_sr == 0){
					accion = WALK;
				}
			}
		//WALK = 0
		//TURN_L = 2
		//TURN_SR = 3
		//acciones.push_back(accion);
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
			//acciones.push_back(accion);
		}else{
			cout << "NUEVO" << endl;
			contador0++;
			if(contador0 == 1){
				accion = TURN_L;
			}else if(contador0 == 2){			//Segunda vez que nos encontramos con nada de forma seguida 
				accion = TURN_SR;
				giro45izq = 3;			//Giramos 90 grados a derecha, antes que ir para atras con el turn_l  otra vez
			}else if(contador0 == 3){	//No hay na a la derecha, volvemos al rumbo del backtrack
				accion = TURN_L;
			}
		}
	}else{ 
		char i = ViablePorAlturaR(sensores.superficie[1], sensores.cota[1] - sensores.cota[0], tiene_zapatillas, WALK);
		char c = ViablePorAlturaR(sensores.superficie[2], sensores.cota[2] - sensores.cota[0], tiene_zapatillas, WALK);
		char d = ViablePorAlturaR(sensores.superficie[3], sensores.cota[3] - sensores.cota[0], tiene_zapatillas, WALK);
		
		//Chocan agentes
		if(sensores.agentes[2] == 'r'){
			c = 'P';
		}	
		int pos = VeoCasillaInteresante_nivel1(i, c, d, sensores);

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
						accion = TURN_L;
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
			//acciones.push_back(accion);
	}
	//Devolver accion
	ultima_accion = accion;
	visitadosR.emplace(sensores.posF, sensores.posC);

	return accion;
}

void AnularMatrizR(vector<vector<unsigned char>>& matriz){
	for(auto& fila : matriz){
		for(auto& val : fila){
			val = 0;
		}
	}
}

void ComportamientoRescatador::VisualizaPlan(const EstadoR &st, const list<Action> &plan) {
    AnularMatrizR(mapaConPlan);
    EstadoR cst = st;

    auto it = plan.begin();
    while (it != plan.end()) {
        switch (*it) {
            case WALK:
                switch (cst.rumbo) {
                    case 0:
                        cst.f--;
                        break;
                    case 1:
                        cst.f--;
                        cst.c++;
                        break;
                    case 2:
                        cst.c++;
                        break;
                    case 3:
                        cst.f++;
                        cst.c++;
                        break;
                    case 4:
                        cst.f++;
                        break;
                    case 5:
                        cst.f++;
                        cst.c--;
                        break;
                    case 6:
                        cst.c--;
                        break;
                    case 7:
                        cst.f--;
                        cst.c--;
                        break;
                }
                mapaConPlan[cst.f][cst.c] = 2;
                break;

            case TURN_SR:
                cst.rumbo = (cst.rumbo + 1) % 8;
                break;
			case TURN_L:
				cst.rumbo = (cst.rumbo + 6) % 8;
				break;
		}
        it++;
    }
}


bool ComportamientoRescatador::Find(const NodoR& st, const list<NodoR>&lista){
	for(auto it = lista.begin(); it != lista.end(); ++it){
		if(st == (*it)) return true;
	}
	return false;
}

EstadoR NextCasillaRescatador(const EstadoR &st, Action accion){
    EstadoR siguiente = st;
	switch(accion){
		case WALK: 
			switch (st.rumbo) {
    	    	case norte:
    	    	    siguiente.f = st.f - 1;
    	    	    break;
    	    	case noreste:
    	    	    siguiente.f = st.f - 1;
    	    	    siguiente.c = st.c + 1;
    	    	    break;
    	    	case este:
    	    	    siguiente.c = st.c + 1;
    	    	    break;
    	    	case sureste:
    	    	    siguiente.f = st.f + 1;
    	    	    siguiente.c = st.c + 1;
    	    	    break;
    	    	case sur:
    	    	    siguiente.f = st.f + 1;
    	    	    break;
    	    	case suroeste:
    	    	    siguiente.f = st.f + 1;
    	    	    siguiente.c = st.c - 1;
    	    	    break;
    	    	case oeste:
    	    	    siguiente.c = st.c - 1;
    	    	    break;
    	    	case noroeste:
    	    	    siguiente.f = st.f - 1;
    	    	    siguiente.c = st.c - 1;
    	    	    break;
    		}
			break;
		case RUN:
			switch (st.rumbo){
    	    	case norte:
    	    	    siguiente.f = st.f - 2;
    	    	    break;
    	    	case noreste:
    	    	    siguiente.f = st.f - 2;
    	    	    siguiente.c = st.c + 2;
    	    	    break;
    	    	case este:
    	    	    siguiente.c = st.c + 2;
    	    	    break;
    	    	case sureste:
    	    	    siguiente.f = st.f + 2;
    	    	    siguiente.c = st.c + 2;
    	    	    break;
    	    	case sur:
    	    	    siguiente.f = st.f + 2;
    	    	    break;
    	    	case suroeste:
    	    	    siguiente.f = st.f + 2;
    	    	    siguiente.c = st.c - 2;
    	    	    break;
    	    	case oeste:
    	    	    siguiente.c = st.c - 2;
    	    	    break;
    	    	case noroeste:
    	    	    siguiente.f = st.f - 2;
    	    	    siguiente.c = st.c - 2;
    	    	    break;
    		}
			break;
    }
    return siguiente;
}


bool CasillaAccesibleRescatador(const EstadoR &st, const vector<vector<unsigned char>>& terreno, const vector<vector<unsigned char>>& altura, Action accion){
	EstadoR next = NextCasillaRescatador(st, accion);
	bool check1 = false, check2 = false;
	check1 = terreno[next.f][next.c] != 'P' && terreno[next.f][next.c] != 'M' && terreno[next.f][next.c] != 'B';
	check2 = (ViablePorAlturaR(terreno[next.f][next.c], altura[st.f][st.c] - altura[next.f][next.c], st.zapatillas, accion) != 'P');
	return check1 && check2;
}

EstadoR applyR(Action accion, const EstadoR& st, const vector<vector<unsigned char>> &terreno,
	const vector<vector<unsigned char>> &altura){
	EstadoR next = st;
	switch(accion){
		case WALK:
			if (CasillaAccesibleRescatador(st, terreno, altura, accion)){
				next = NextCasillaRescatador(st, accion);
			}
			break;
		case TURN_SR:
			next.rumbo = (next.rumbo + 1) % 8;
			break;
		case TURN_L:
			next.rumbo = (next.rumbo + 6) % 8;
			break;
			
	}
	return next;
}



//Devuelve el coste asosciado a la ejecucion de la accion dada como argumento sobre la casilla estado st
int ComportamientoRescatador::CosteEnergia(const EstadoR& st, Action accion, const vector<vector<unsigned char>>& terreno, const vector<vector<unsigned char>>& altura){
	//Cosas a tener en cuenta pal coste
/*
	WALK
	Casilla Inicial Gasto energía Incr/Decr por cambio de altura
'A' 					100 					10
'T' 					20 						5
'S' 					2 						1
Resto de Casillas 		1 						0
RUN (Rescatador)
Casilla Inicial Gasto Normal energía Incr/Decr por cambio de altura
'A' 					150 					15
'T' 					35						5
'S' 					3 						2
Resto de Casillas 1 0
TURN_L (Rescatador)
Casilla Inicial Gasto Normal energía Incr/Decr por cambio de altura
'A' 					30						0
'T' 					5 						0
'S' 					1 						0
Resto de Casillas 1 0
TURN_SR
Casilla Inicial Gasto Normal energía Incr/Decr por cambio de altura
'A' 					16						0
'T' 					3 						0
'S' 					1 						0
Resto de Casillas 		1 						0

casillas transitables = {A,T,S,C,X}

*/	
	int coste = 0;
	EstadoR next = st;
	int diferencia_altura;
	switch(accion){
		case WALK:
			next = NextCasillaRescatador(st, accion);
			if(terreno[next.f][next.c] == 'P' || terreno[next.f][next.c] == 'M' || terreno[next.f][next.c] == 'B') return coste;

			if(ViablePorAlturaR(terreno[next.f][next.c], altura[next.f][next.c] - altura[st.f][st.c], st.zapatillas, WALK) != 'P'){
				diferencia_altura = altura[next.f][next.c] - altura[st.f][st.c];
				switch(terreno[st.f][st.c]){
					case 'A':
						coste += 100;
						coste += (diferencia_altura * 10);
						break;
					case 'T':
						coste += 20;
						coste += (diferencia_altura * 5);
						break;
					case 'S':
						coste += 2;
						coste += (diferencia_altura * 1);
						break;
					default:
						coste += 1;
						break;
				}
			}
			break;
		
			
			
		case TURN_SR:
			switch(terreno[st.f][st.c]){
					case 'A':
						coste += 16;
						break;
					case 'T':
						coste += 3;
						break;
					case 'S':
						coste += 1;
						break;
					default:
						coste += 1;
						break;
			}
			break;

		case TURN_L:
			switch(terreno[st.f][st.c]){
					case 'A':
						coste += 30;
						break;
					case 'T':
						coste += 5;
						break;
					case 'S':
						coste += 1;
						break;
					default:
						coste += 1;
						break;
			}
			break;			
	}

	return coste;
}

class ComparaCosteR{
	public:
		bool operator()(NodoR& n1, NodoR& n2) const{
			return n1.coste > n2.coste;
		}
};

list<Action> ComportamientoRescatador::AnchuraRescatador(const EstadoR& inicio, const EstadoR& final, const vector<vector<unsigned char>>& terreno, const vector<vector<unsigned char>>& altura, 
															const Sensores& sensores){
	NodoR current_node;
	priority_queue<NodoR, vector<NodoR>, ComparaCosteR> frontier;
	set<NodoR> explored;
	list<Action> path;

	current_node.estado = inicio;
	current_node.coste = 0;

	frontier.push(current_node);
	bool SolutionFound = (current_node.estado.f == final.f && current_node.estado.c == final.c);
	while(!SolutionFound && !frontier.empty()){
		frontier.pop();
		explored.insert(current_node);

		//Comprueba si tengo zapas
		if(terreno[current_node.estado.f][current_node.estado.c] == 'D') current_node.estado.zapatillas = true;

		//Genero el hijo resultante de aplicar la accion WALK
		NodoR child_WALK = current_node;
		//Calculo el coste de energia de andar
		child_WALK.coste += CosteEnergia(child_WALK.estado, WALK, terreno, altura);
		child_WALK.estado = applyR(WALK, current_node.estado, terreno, altura);
		if(child_WALK.estado.f == final.f && child_WALK.estado.c == final.c){
			//El hijo generado es solucion
			child_WALK.secuencia.push_back(WALK);
			current_node = child_WALK;
			SolutionFound = true;
		}else if(explored.find(child_WALK) == explored.end() && child_WALK.coste <= sensores.energia){
			//Se mete en la lista frontier despues de añadir a secuencia la accion
			child_WALK.secuencia.push_back(WALK);
			frontier.push(child_WALK);
		}
/*
		//Genero el hijo resultante de aplicar la accion RUN
		if(!SolutionFound){
			NodoR child_RUN = current_node;
			child_RUN.coste += CosteEnergia(child_RUN.estado, TURN_SR, terreno, altura);
			child_RUN.estado = applyR(RUN, current_node.estado, terreno, altura);
			if(explored.find(child_RUN) == explored.end() && child_RUN.coste <= sensores.energia){
				child_RUN.secuencia.push_back(RUN);
				frontier.push(child_RUN);
			}
		}
*/
		//Genero el hijo resultante de aplicar la accion TURN_SR
		if(!SolutionFound){
			NodoR child_TURN_SR = current_node;
			child_TURN_SR.coste += CosteEnergia(child_TURN_SR.estado, TURN_SR, terreno, altura);
			child_TURN_SR.estado = applyR(TURN_SR, current_node.estado, terreno, altura);
			if(explored.find(child_TURN_SR) == explored.end() && child_TURN_SR.coste <= sensores.energia){
				child_TURN_SR.secuencia.push_back(TURN_SR);
				frontier.push(child_TURN_SR);
			}
		}

		//Genero el hijo resultante de aplicar la accion TURN_L
		if(!SolutionFound){
			NodoR child_TURN_L = current_node;
			child_TURN_L.coste += CosteEnergia(child_TURN_L.estado, TURN_L, terreno, altura);
			child_TURN_L.estado = applyR(TURN_L, current_node.estado, terreno, altura);
			if(explored.find(child_TURN_L) == explored.end() && child_TURN_L.coste <= sensores.energia){
				child_TURN_L.secuencia.push_back(TURN_L);
				frontier.push(child_TURN_L);
			}
		}

		//Paso a evaluar el siguiente nodo en la lista "frontier"
		if(!SolutionFound && !frontier.empty()){
			current_node = frontier.top();
			while(explored.find(current_node) != explored.end() && !frontier.empty()){
				frontier.pop();
				current_node = frontier.top();
			}
		}
	}

	//Devuelvo el camino si hay solucion
	if(SolutionFound) path = current_node.secuencia;

	return path;
}

Action ComportamientoRescatador::ComportamientoRescatadorNivel_2(Sensores sensores)
{
	Action accion;

	if(!hay_plan){
		EstadoR inicio, fin;
		inicio.f = sensores.posF;
		inicio.c = sensores.posC;
		inicio.rumbo = sensores.rumbo;
		inicio.zapatillas = tiene_zapatillas;
		
		fin.f = sensores.destinoF;
		fin.c = sensores.destinoC;
		plan = AnchuraRescatador(inicio, fin, mapaResultado, mapaCotas, sensores);
		VisualizaPlan(inicio, plan);
		hay_plan = plan.size() != 0;
	}
	if(hay_plan && plan.size() > 0){
		accion = plan.front();
		plan.pop_front();
	}
	if(plan.size() == 0){
		hay_plan = false;
	}

	return accion;
	
}

Action ComportamientoRescatador::ComportamientoRescatadorNivel_3(Sensores sensores)
{
}

Action ComportamientoRescatador::ComportamientoRescatadorNivel_4(Sensores sensores)
{
}
