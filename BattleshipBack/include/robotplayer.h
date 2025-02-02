#ifndef ROBOTPLAYER_H
#define ROBOTPLAYER_H

#include <iostream>
#include "player.h"
#include <vector>
#include <utility>

using namespace std;

class RobotPlayer :  public Player{
	private:
		
	public:
		RobotPlayer();
		~RobotPlayer(){}
};

#endif

/*

						Detalhamento dos níveis de alerta

Nível 0:
	O robô irá atacar posições aleatórias do tabuleiro inimigo. nextAttacks é está vazio.
Nível 1:
	O robô atingiu um navio na última jogada, agora irá atacar de forma estratégica usando nextAttacks.
Nível 2:
	O robô atingiu mais uma posição de um navio ainda não destruído e agora sabe sua orientação (vertical/horizontal).
Nível 3:
	Muitas posições já foram atacadas e restam apenas navios com tamanho maior que 1 no tabuleiro inimigo,
	ele se aproveitará desse fato e irá atacar de forma estratégica usando nextAttacks.


*/