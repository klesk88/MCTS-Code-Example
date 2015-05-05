//////////////////////////////////////////////////////////////////////////////////////////////
// @author: Michele Ermacora
//
// This class is use for manage the level display on the console.
//////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _GAME_FIELD_H
#define _GAME_FIELD_H

#include <iostream>
#include <cmath>
#include <ctime>
#include <Windows.h>
#include <vector>

#include "DataStructures.h"
#include "SharedFunctions.h"

using namespace std;

class CGameField
{
public:
	//Pass the dimensions of the game field to generate plus
	//the number of obstacles
	CGameField(const unsigned short kusHeight, 
			   const unsigned short kusWidth,
			   const unsigned short kusNumberOfObstacles);
	CGameField(const CGameField *kpOldObj);
	~CGameField();
	void ClearPosition(const unsigned short kusY,
					   const unsigned short kusX);
	void ClearPosition(const SGCoordinates2D<unsigned short> *const kpkPositionToClean);
	//Check if the position is free. Return true if it is
	bool IsPositionFree(const SGCoordinates2D<unsigned short> *const kpkPosition) const;
	bool IsPositionFree(const unsigned short kusY,
					    const unsigned short kusX) const;
	void DisplayLevel();
	//Display message when the game is over
	void GameOver(const char *const kpkcMessage);
	//Method used for update the game field by placing at y,x position the value kcValue
	void ModifyGameField(const unsigned short kusY, 
						 const unsigned short kusX,
						 const char kcValue);
	void UpdateLevel(const char kcName,
					 SGCoordinates2D<unsigned short> *const kpkActualPosition);
	const char* const* GetConstGameField() const;
	const SGCoordinates2D<unsigned short> *GetExitPosition() const;
	char GetExitLetter() const;
	char GetFieldBorderLetter() const;
	unsigned short GetHeight() const;
	unsigned short GetWidth() const;	
private:
	//method that adds to the level the obstacles and the exit. 
	void AddContent();
	//from http://www.cplusplus.com/articles/4z18T05o/
	void ClearScreen();
private:	
	static const char m_kcExit = 'E';
	static const char m_kcBorderObstaclesChar = 'X';
	const unsigned short m_kusNumberOfObstacles;
	SGCoordinates2D<unsigned short> *m_pExitPosition;
	CGMatrix<char> *m_pGameMap;
//DEBUG FUNCTIONS regarding the draw of the MCTS path
public:
	void DrawMCTSDebugPath(const vector<SGCoordinates2D<unsigned short>* const> *const kpkvDebugPath, 
						   const char kcDebugCharacter);
	void CleanMCTSDebugPath(const vector<SGCoordinates2D<unsigned short>* const> *const kpkvdebugPath);
};

#endif