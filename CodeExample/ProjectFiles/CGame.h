//////////////////////////////////////////////////////////////////////////////////////////////
// @author: Michele Ermacora
//
// This class represents the game. It contains the actual level render to the console, as well
// as the instances of the prisoner and the guard. 
//////////////////////////////////////////////////////////////////////////////////////////////

#ifndef GAME_H
#define GAME_H


#include <cstdlib> 
#include <thread>

#include "DataStructures.h"
#include "Players\CPrisoner.h"
#include "Players\CGuard.h"
#include "CGameField.h"

using namespace std;

class CGame
{
public:
	CGame();
	~CGame();
public:
	void Init(const unsigned short kusHeight, 
			  const unsigned short kusWidth,
			  const unsigned short kusNumberOfObstacles);
	void DisplayLevel();
	void GameLoop();
private:
	bool ExitGame();
private:
	CGameField *m_pGameMap;
	CGuard *m_pGuard;
	CPrisoner *m_pPrisoner;
};

#endif
