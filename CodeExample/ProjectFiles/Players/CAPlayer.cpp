//////////////////////////////////////////////////////////////////////////////////////////////
// @author: Michele Ermacora
//////////////////////////////////////////////////////////////////////////////////////////////

#include "CAPlayer.h"
#include "..\CheckMemoryLeaks.h"

CAPlayer::CAPlayer(const char kcName,
				   const CGameField *const kpkRowLevel)
			  : m_kcName(kcName),
			    m_kRowLevel(kpkRowLevel)
{
	this->m_pActualPosition= new SGCoordinates2D<unsigned short>();
	this->m_pOldPosition = new SGCoordinates2D<unsigned short>();
	this->m_bHasToSurrender = false;
}

CAPlayer::~CAPlayer()
{
	delete this->m_pActualPosition;
	delete this->m_pOldPosition;
}

void CAPlayer::AddPlayerToLevel(CGameField *const pkRowLevel,
							    const SGCoordinates2D<unsigned short> *const kpkPositionToCompare,
								const int kiMinDistBetweenPositions,
								const int kiMaxDistBetweenPositions)
{
	srand((unsigned int)time(0));
	const unsigned int kuiHeight = pkRowLevel->GetHeight();
	const unsigned int kuiWidth = pkRowLevel->GetWidth();
	const int kiDistance = kiMaxDistBetweenPositions - kiMinDistBetweenPositions;
	if(kiDistance < 0)
	{
		throw logic_error("Minimum distance is smaller the maximum distance");
	}
	do
	{
		if((kpkPositionToCompare->y + kiMaxDistBetweenPositions) < ((signed)kuiHeight - 1))
		{
			this->m_pActualPosition->y = kpkPositionToCompare->y  + kiMinDistBetweenPositions + (rand() % kiDistance);
		}
		else
		{
			this->m_pActualPosition->y = kpkPositionToCompare->y  - kiMinDistBetweenPositions - (rand() % kiDistance);
		}
		if((kpkPositionToCompare->x + kiMaxDistBetweenPositions) < ((signed)kuiWidth - 1))
		{
			this->m_pActualPosition->x = kpkPositionToCompare->x  + kiMinDistBetweenPositions + (rand() % kiDistance);
		}
		else
		{
			this->m_pActualPosition->x = kpkPositionToCompare->x  - kiMinDistBetweenPositions - (rand() % kiDistance);
		}
	}while(!pkRowLevel->IsPositionFree(this->m_pActualPosition));
	pkRowLevel->ModifyGameField(this->m_pActualPosition->y, this->m_pActualPosition->x, this->m_kcName);
}

const char* CAPlayer::GetName() const
{
	return &this->m_kcName;
}

SGCoordinates2D<unsigned short> *CAPlayer::GetActualPosition() const
{
	return this->m_pActualPosition;
}

SGCoordinates2D<unsigned short> *CAPlayer::GetOldPosition() const
{
	return this->m_pOldPosition;
}


bool CAPlayer::GetSurrender() const
{
	return this->m_bHasToSurrender;
}