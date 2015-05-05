//////////////////////////////////////////////////////////////////////////////////////////////
// @author: Michele Ermacora
//
// Abstract class that describes a common player
//////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _PLAYER_H
#define _PLAYER_H

#include"..\DataStructures.h"
#include"..\CGameField.h"

class CAPlayer 
{
public:
	CAPlayer::CAPlayer(const char kcName,
					   const CGameField *const kpkRowLevel);
	~CAPlayer();
	//The method add the player to the game map. It will initialize his
	//position, as well as modify the passed level (first parameter) 
	void AddPlayerToLevel(CGameField *const pkRowLevel,
				   const SGCoordinates2D<unsigned short> *const kpkPositionToCompare,
				   const int kiMinDistBetweenPositions,
				   const int kiMaxDistBetweenPositions);
	//Name of the player
	const char* GetName() const;
	SGCoordinates2D<unsigned short> *GetActualPosition() const;
	SGCoordinates2D<unsigned short> *GetOldPosition() const;
	//Method that the child classes need to override
	virtual void Update(const SGCoordinates2D<unsigned short> *const kpkEnemyPosition) = 0;
	bool GetSurrender() const;
protected:
	SGCoordinates2D<unsigned short> *m_pActualPosition;
	SGCoordinates2D<unsigned short> *m_pOldPosition;
	//Variable use for store the row level when no players are still add to it.
	//Use for keep a representation of the structure of the level, and perform
	//reasoning on it.
	const CGameField m_kRowLevel; 
	bool m_bHasToSurrender;
private:
	//character used for display the player inside the game field
	const char m_kcName;
};

#endif