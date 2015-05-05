//////////////////////////////////////////////////////////////////////////////////////////////
// @author: Michele Ermacora
//////////////////////////////////////////////////////////////////////////////////////////////

#include <map>

#include "CMCTSNodeGuard.h"
#include "..\CheckMemoryLeaks.h"

CMCTSNodeGuard::CMCTSNodeGuard(SInfoForNodeInit info,
							   CMCTSNodeGuard *const pkParent)
					:CGAMCTSNode(info, pkParent)
{
	this->m_bLooseConditionReached = false;
	this->m_bWinConditionReached = false;
	if(PrisonerCatched(info.m_kpkActualPosition, info.m_kpkOpponentPosition))
	{
		this->m_bWinConditionReached = true;
	}
	if(PrisonerEscaped(info.m_kpkOpponentPosition))
	{
		this->m_bLooseConditionReached = true;
	}
	this->m_dDistanceFromPrisoner = CalculateEuclideanDistance(info.m_kpkActualPosition,info.m_kpkOpponentPosition);
}

CMCTSNodeGuard::~CMCTSNodeGuard()
{

}

CMCTSNodeGuard* CMCTSNodeGuard::ExpandChildren()
{
	SGCoordinates2D<unsigned short> opponentPosition(this->m_kpkOpponentPosition);
	NewPositionCloserToTarget(&opponentPosition, this->m_kpkLevel->GetExitPosition(), this->m_kpkLevel);
	AddChild(&opponentPosition);
	return this->m_vChildrensExpanded.at(this->m_vChildrensExpanded.size() - 1);
}

bool CMCTSNodeGuard::PrisonerCatched(const SGCoordinates2D<unsigned short> *const kpkGuardPosition, 
								   const SGCoordinates2D<unsigned short> *const kpkFugitivePosition) const
{
	if(*kpkGuardPosition == kpkFugitivePosition 
	 && *kpkGuardPosition != this->m_kpkLevel->GetExitPosition())
	{
		return true;
	}
	return false;
}

bool CMCTSNodeGuard::PrisonerEscaped(const SGCoordinates2D<unsigned short> *const kpkPrisonerPosition) const
{
	if(*kpkPrisonerPosition == this->m_kpkLevel->GetExitPosition())
	{
		return true;
	}
	return false;
}

bool CMCTSNodeGuard::WantsToSurrender() const
{
	if(this->m_dQ < 0.2 
		&& CalculateEuclideanDistance(this->m_kpkOpponentPosition, this->m_kpkLevel->GetExitPosition()) > 3)
	{
		return true;
	}
	return false;
}

bool CMCTSNodeGuard::IsTerminal() const
{
	if(this->m_bLooseConditionReached || this->m_bWinConditionReached)
	{
		return true;
	}
	return false;
}

void CMCTSNodeGuard::NewRndPositionCloserToTarget(SGCoordinates2D<unsigned short> *const pkSourcePosition, 
												  const SGCoordinates2D<unsigned short> *const kpkTargetPosition)
{
	SGCoordinates2D<unsigned short> newSourcePosition = SGCoordinates2D<unsigned short>();
	multimap<double, SGCoordinates2D<unsigned short>> mmDistanceTable;
	for(int i = 0 ; i < (int)ECMOVES::Count; i++)
	{
		newSourcePosition = *pkSourcePosition;
		ECMOVES eMove = (ECMOVES)(i);
		switch(eMove)
		{
		case ECMOVES::UP:
			newSourcePosition.y--;
			break;
		case ECMOVES::DOWN:
			newSourcePosition.y++;
			break;
		case ECMOVES::LEFT:
			newSourcePosition.x--;
			break;
		case ECMOVES::RIGHT:
			newSourcePosition.x++;
			break;
		default : 
			throw logic_error("value not inside the enum range");
		}
		if(!this->m_kpkLevel->IsPositionFree(&(newSourcePosition)))
		{
			continue;
		}
		double dNewDistance = CalculateEuclideanDistance(&newSourcePosition, kpkTargetPosition);
		mmDistanceTable.insert(pair<double, SGCoordinates2D<unsigned short>>(dNewDistance, newSourcePosition));
	}	
	std::multimap<double, SGCoordinates2D<unsigned short>>::const_iterator pkIt = mmDistanceTable.begin();
	srand((unsigned int)time(0));
	int iRange = (rand() %  100);
	//select randomly if take the first or the second move closer to the target
	if(iRange > 49)
	{
		pkIt++;
	}
	*pkSourcePosition = (*pkIt).second;
	mmDistanceTable.clear();
}

double CMCTSNodeGuard::Simulation(const unsigned short kusNumberOfSimulations)
{
	double dDeltaValue = 0;
	if(this->m_bWinConditionReached)
	{
		return 1 ;
	}
	if(this->m_bLooseConditionReached)
	{
		return 0;
	}
	SGCoordinates2D<unsigned short> guardPositionCpy(this->m_kpkActualPosition);
	SGCoordinates2D<unsigned short> prisonerPositionCpy(this->m_kpkOpponentPosition);
	//scale down the delta value calculated at each simulation step, by the distance between
	//the position of the guard in this node and the position of the prisoner in this node
	double dScaleDownFactor =  (this->m_dDistanceFromPrisoner / 100);
	for(int i = 0; i < kusNumberOfSimulations ; i++)
	{
		NewRndPositionCloserToTarget(&guardPositionCpy, &prisonerPositionCpy);
		NewPositionCloserToTarget(&prisonerPositionCpy, this->m_kpkLevel->GetExitPosition(), this->m_kpkLevel);
		double dDistanceGuardTheft = CalculateEuclideanDistance(&guardPositionCpy, &prisonerPositionCpy);
		if(PrisonerCatched(&guardPositionCpy, &prisonerPositionCpy))
		{
			dDeltaValue += 1 * (kusNumberOfSimulations - i);
			break;
		}
		if(PrisonerEscaped(&prisonerPositionCpy))
		{
			break;
		}
		//if the gurd is in a diagonal position compared to the prisoner. Best
		//position, as it have a chance to catch it
		if(dDistanceGuardTheft == sqrt(2))
		{
			dDeltaValue += 0.7 - dScaleDownFactor;
			continue;
		}
		dDeltaValue += 0.3 - dScaleDownFactor;	
	}
	// keep the delta value between 0 and 1
	dDeltaValue /= (double)kusNumberOfSimulations;
	return dDeltaValue;
}
