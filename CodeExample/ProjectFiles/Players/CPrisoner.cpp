//////////////////////////////////////////////////////////////////////////////////////////////
// @author: Michele Ermacora
//////////////////////////////////////////////////////////////////////////////////////////////

#include "CPrisoner.h"
#include "..\CheckMemoryLeaks.h"

CPrisoner::CPrisoner(const char kcName,
					 const CGameField *const kpkRowLevel)
			:CAPlayer(kcName, kpkRowLevel)
{
	this->m_pMcts = new CGAMCTS<CMCTSNodePrisoner>(1/sqrt(2), 15, 0.3f);
}

CPrisoner::~CPrisoner()
{
	delete this->m_pMcts;
}

void CPrisoner::Update(const SGCoordinates2D<unsigned short> *const kpkEnemyPosition)
{
	*(this->m_pOldPosition) = (*this->m_pActualPosition);
	this->m_pMcts->StartMCTS(&this->m_kRowLevel, this->m_pActualPosition, kpkEnemyPosition, &(this->m_bHasToSurrender));
}

const vector<SGCoordinates2D<unsigned short>* const> *CPrisoner::GetDebugPath() const
{
	return this->m_pMcts->GetDebugPath();
}

