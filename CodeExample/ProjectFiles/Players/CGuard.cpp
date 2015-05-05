//////////////////////////////////////////////////////////////////////////////////////////////
// @author: Michele Ermacora
//////////////////////////////////////////////////////////////////////////////////////////////

#include "CGuard.h"
#include "..\CheckMemoryLeaks.h"

CGuard::CGuard(const char kcName,
			   const CGameField *const kpkRowLevel)
		:CAPlayer(kcName, kpkRowLevel)
{
	this->m_pMcts = new CGAMCTS<CMCTSNodeGuard>(1/sqrt(2), 5, 0.3f);
}

CGuard::~CGuard()
{
	delete this->m_pMcts;
}

void CGuard::Update(const SGCoordinates2D<unsigned short> *const kpkEnemyPosition)
{
	*(this->m_pOldPosition) = (*this->m_pActualPosition);
	this->m_pMcts->StartMCTS(&this->m_kRowLevel, this->m_pActualPosition, kpkEnemyPosition,&(this->m_bHasToSurrender));
}

const vector<SGCoordinates2D<unsigned short>* const> *CGuard::GetDebugPath() const
{
	return this->m_pMcts->GetDebugPath();
}

