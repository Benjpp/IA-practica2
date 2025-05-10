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
		accion = ComportamientoAuxiliarNivel_1 (sensores);
		break;
	case 2:
		//accion = ComportamientoAuxiliarNivel_2 (sensores);
		break;
	case 3:
		//accion = ComportamientoAuxiliarNivel_3 (sensores);
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

int ComportamientoAuxiliar::VeoCasillaInteresante_nivel0(char i, char c, char d, Sensores sensores){
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

void SituarSensorEnMapaA(vector<vector<unsigned char>> &m, vector<vector<unsigned char>> &a, Sensores sensores)
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

Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_0(Sensores sensores)
{
	
	Action accion;
	acciones.push_back(ultima_accion);
	visitadosA.emplace(sensores.posF, sensores.posC);

	SituarSensorEnMapaA(mapaResultado, mapaCotas, sensores);
	// El comportamiento de seguir un camino hasta encontrar un puesto base.
	if(sensores.superficie[0] == 'D') tiene_zapatillas = true;

	//Comportamiento
	if(sensores.superficie[0] == 'X'){					//He llegado al puesto base
		accion = IDLE;
	}else if(giro45izq > 0){
		accion = TURN_SR;
		giro45izq--;
		//acciones.push_back(accion);
	}else if(backtrack == true){
		char i = ViablePorAlturaA(sensores.superficie[1], sensores.cota[1] - sensores.cota[0]);
		char c = ViablePorAlturaA(sensores.superficie[2], sensores.cota[2] - sensores.cota[0]);
		char d = ViablePorAlturaA(sensores.superficie[3], sensores.cota[3] - sensores.cota[0]);
			
		while(acciones[backtrack_it] == IDLE) backtrack_it--;
		
		if(sensores.agentes[2] == 'r'){
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
			}else if(acciones[backtrack_it] == TURN_SR){
				giro45izq = (8 - ahorro_giro()) - 1;
				accion = TURN_SR;
			}
			//cout << " OL" << endl;
			//WALK = 0
			//TURN_L = 2
			//TURN_SR = 3
			//acciones.push_back(accion);
		}else if(pos != 0){
			//cout << "Adios back" << endl;
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
			//acciones.push_back(accion);
		}else{
			//cout << "NUEVO" << endl;
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
		int pos = VeoCasillaInteresante_nivel0(i, c, d, sensores);

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
			//acciones.push_back(accion);
	}
	//Devolver accion
	ultima_accion = accion;

	return accion;
}

int ComportamientoAuxiliar::VeoCasillaInteresante_nivel1(char i, char c, char d, Sensores sensores){

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
	if((c == 'C' || c == 'S') && visitadosA.find(coordenada2) == visitadosA.end()) return 2;
	else if((i == 'C' || i == 'S') && visitadosA.find(coordenada1) == visitadosA.end()) return 1;
	else if((d == 'C' || d == 'S') && visitadosA.find(coordenada3) == visitadosA.end()) return 3;
	else return 0;
}

Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_1(Sensores sensores)
{
	Action accion;
	SituarSensorEnMapaA(mapaResultado, mapaCotas, sensores);
	acciones.push_back(ultima_accion);
	visitadosA.emplace(sensores.posF, sensores.posC);

	if(sensores.superficie[0] == 'D') tiene_zapatillas = true;

	if(giro45izq > 0){
		accion = TURN_SR;
		giro45izq--;
		acciones.push_back(accion);
	}else if(backtrack == true){
		char i = ViablePorAlturaA(sensores.superficie[1], sensores.cota[1] - sensores.cota[0]);
		char c = ViablePorAlturaA(sensores.superficie[2], sensores.cota[2] - sensores.cota[0]);
		char d = ViablePorAlturaA(sensores.superficie[3], sensores.cota[3] - sensores.cota[0]);
			
		while(acciones[backtrack_it] == IDLE) backtrack_it--;
		
		if(sensores.agentes[2] == 'r'){
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
			}else if(acciones[backtrack_it] == TURN_SR){
				giro45izq = (8 - ahorro_giro()) - 1;
				accion = TURN_SR;
			}
			//cout << " OL" << endl;
			//WALK = 0
			//TURN_L = 2
			//TURN_SR = 3
			//acciones.push_back(accion);
		}else if(pos != 0){
			//cout << "Adios back" << endl;
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
			//acciones.push_back(accion);
		}else{
			//cout << "NUEVO" << endl;
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
			//acciones.push_back(accion);
	}
	//Devolver accion
	ultima_accion = accion;

	return accion;
}

void AnularMatrizA(vector<vector<unsigned char>>& matriz){
	for(auto& fila : matriz){
		for(auto& val : fila){
			val = 0;
		}
	}
}

void ComportamientoAuxiliar::VisualizaPlan(const EstadoA &st, const list<Action> &plan) {
    AnularMatrizA(mapaConPlan);
    EstadoA cst = st;

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
        }
        it++;
    }
}


bool ComportamientoAuxiliar::Find(const NodoA& st, const list<NodoA>&lista){
	for(auto it = lista.begin(); it != lista.end(); ++it){
		if(st == (*it)) return true;
	}
	return false;
}

EstadoA NextCasillaAuxiliar(const EstadoA &st){
    EstadoA siguiente = st;
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
    return siguiente;
}


bool CasillaAccesibleAuxiliar(const EstadoA &st, const vector<vector<unsigned char>>& terreno, const vector<vector<unsigned char>>& altura){
	EstadoA next = NextCasillaAuxiliar(st);
	bool check1 = false, check2 = false, check3 = false;
	check1 = terreno[next.f][next.c] != 'P' && terreno[next.f][next.c] != 'M';
	check2 = terreno[next.f][next.c] != 'B' || (terreno[next.f][next.c] == 'B' && st.zapatillas);
	check3 = abs(altura[next.f][next.c] - altura[st.f][st.c]) <= 1;

	return check1 && check2 && check3;
}

EstadoA applyA(Action accion, const EstadoA& st, const vector<vector<unsigned char>> &terreno,
	const vector<vector<unsigned char>> &altura){
	EstadoA next = st;
	switch(accion){
		case WALK:
			if (CasillaAccesibleAuxiliar(st,terreno,altura)){
				next = NextCasillaAuxiliar(st);
			}
			break;
		case TURN_SR:
			next.rumbo = (next.rumbo + 1) % 8;
			break;
	}
	return next;
}

int ComportamientoAuxiliar::CosteEnergia(const EstadoA& st, Action accion, const vector<vector<unsigned char>>& terreno, const vector<vector<unsigned char>>& altura){
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
	int coste_energia = 0;
	switch(accion){
		case WALK:
			switch(terreno[st.f][st.c]){
				case 'A':
					coste_energia += 100;
					
					break;
				case 'T':

				break;
				case 'S':

				break;

			}
	}

	return coste_energia;
}

list<Action> ComportamientoAuxiliar::AnchuraAuxiliar(const EstadoA& inicio, const EstadoA& final,
															const vector<vector<unsigned char>>& terreno, const vector<vector<unsigned char>>& altura){
	NodoA current_node;
	list<NodoA> frontier;
	list<NodoA> explored;
	list<Action> path;

	current_node.estado = inicio;

	frontier.push_back(current_node);
	bool SolutionFound = (current_node.estado.f == final.f && current_node.estado.c == final.c);
	while(!SolutionFound && !frontier.empty()){
		frontier.pop_front();
		explored.push_back(current_node);

		//Comprueba si tengo zapas
		if(terreno[current_node.estado.f][current_node.estado.c] == 'D') current_node.estado.zapatillas = true;

		//Genero el hijo resultante de aplicar la accion WALK
		NodoA child_WALK = current_node;
		//Calculo el coste de energia de andar
		int coste_WALK = CosteEnergia(child_WALK.estado, WALK, terreno, altura);
		child_WALK.estado = applyA(WALK, current_node.estado, terreno, altura);
		if(child_WALK.estado.f == final.f && child_WALK.estado.c == final.c){
			//El hijo generado es solucion
			child_WALK.secuencia.push_back(WALK);
			current_node = child_WALK;
			SolutionFound = true;
		}else if(!Find(child_WALK, frontier) && !Find(child_WALK, explored)){
			//Se mete en la lista frontier despues de añadir a secuencia la accion
			child_WALK.secuencia.push_back(WALK);
			frontier.push_back(child_WALK);
		}

		//Genero el hijo resultante de aplicar la accion TURN_SR
		if(!SolutionFound){
			NodoA child_TURN_SR = current_node;
			child_TURN_SR.estado = applyA(TURN_SR, current_node.estado, terreno, altura);
			if(!Find(child_TURN_SR, frontier) && !Find(child_WALK, explored)){
				child_TURN_SR.secuencia.push_back(TURN_SR);
				frontier.push_back(child_TURN_SR);
			}
		}

		//Paso a evaluar el siguiente nodo en la lista "frontier"
		if(!SolutionFound && !frontier.empty()){
			current_node = frontier.front();
			SolutionFound = (current_node.estado.f == final.f && current_node.estado.c == final.c);
		}
	}

	if(SolutionFound) path = current_node.secuencia;

	return path;
}

Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_2(Sensores sensores)
{
}

Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_3(Sensores sensores)
{
	Action accion;

	if(!hay_plan){
		EstadoA inicio, fin;
		inicio.f = sensores.posF;
		inicio.c = sensores.posC;
		inicio.rumbo = sensores.rumbo;
		inicio.zapatillas = tiene_zapatillas;
		fin.f = sensores.destinoF;
		fin.c = sensores.destinoC;
		plan = AnchuraAuxiliar(inicio, fin, mapaResultado, mapaCotas);
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

Action ComportamientoAuxiliar::ComportamientoAuxiliarNivel_4(Sensores sensores)
{
}
