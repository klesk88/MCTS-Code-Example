//////////////////////////////////////////////////////////////////////////////////////////////
// @author: Michele Ermacora
//
// Specialization of the class CAPlayer representing the Prisoner
//////////////////////////////////////////////////////////////////////////////////////////////

#ifndef THIEF_H
#define THIEF_H

#include "..\MCTS\CGAMCTS.h"
#include "..\MCTSNode\CMCTSNodePrisoner.h"
#include "CAPlayer.h"

class CPrisoner : public CAPlayer
{
public:
	CPrisoner::CPrisoner(const char kcName,
					     const CGameField *const kpkRowLevel);
	~CPrisoner();
	//Get the path given by the MCTS
	const vector<SGCoordinates2D<unsigned short>* const> *GetDebugPath() const;
	//Call MCTS for find the next move to perform
	void Update(const SGCoordinates2D<unsigned short> *const kpkEnemyPosition);
private:
	CGAMCTS<CMCTSNodePrisoner> *m_pMcts;
	
};

#endif