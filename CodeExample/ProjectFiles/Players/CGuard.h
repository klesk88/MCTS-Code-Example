//////////////////////////////////////////////////////////////////////////////////////////////
// @author: Michele Ermacora
//
// Specialization of the class CAPlayer representing the Guard
//////////////////////////////////////////////////////////////////////////////////////////////

#ifndef GUARD_H
#define GUARD_H

#include "..\MCTS\CGAMCTS.h"
#include "..\MCTSNode\CMCTSNodeGuard.h"
#include "CAPlayer.h"

class CGuard : public CAPlayer
{
public:
	CGuard::CGuard(const char kcName,
				   const CGameField *const kpkRowLevel);
	~CGuard();
	//get the MCTS debug path
	const vector<SGCoordinates2D<unsigned short>* const> *GetDebugPath() const;
	//Call MCTS for find the next move to perform
	void Update(const SGCoordinates2D<unsigned short> *const kpkEnemyPosition);
private:
	CGAMCTS<CMCTSNodeGuard> *m_pMcts;
};

#endif