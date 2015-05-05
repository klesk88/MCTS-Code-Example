//////////////////////////////////////////////////////////////////////////////////////////////
// @author: Michele Ermacora
//////////////////////////////////////////////////////////////////////////////////////////////

#include "CMCTSNodePrisoner.h"
#include "..\CheckMemoryLeaks.h"

CMCTSNodePrisoner::CMCTSNodePrisoner(SInfoForNodeInit info, 
									 CMCTSNodePrisoner *const pkParent)
							:CGAMCTSNode(info, pkParent)
{
	this->m_bLooseConditionReached = false;
	this->m_bWinConditionReached = false;
	if(FinalConditionMet(info.m_kpkActualPosition))
	{
		this->m_bWinConditionReached = true;
	}
	if(GuardCatch(info.m_kpkActualPosition, info.m_kpkOpponentPosition))
	{
		this->m_bLooseConditionReached = true;
	}
	
}

CMCTSNodePrisoner::~CMCTSNodePrisoner()
{

}

CMCTSNodePrisoner* CMCTSNodePrisoner::ExpandChildren() 
{
	SGCoordinates2D<unsigned short> opponentPosition(this->m_kpkOpponentPosition);
	NewPositionCloserToTarget(&opponentPosition, this->m_kpkActualPosition,  this->m_kpkLevel);
	AddChild(&opponentPosition);
	return this->m_vChildrensExpanded.at(this->m_vChildrensExpanded.size() - 1);
}

bool CMCTSNodePrisoner::FinalConditionMet(const SGCoordinates2D<unsigned short> *const kpkPrisonerPosition) const
{
	if(*kpkPrisonerPosition == this->m_kpkLevel->GetExitPosition())
	{
		return true;
	}
	return false;
}

bool CMCTSNodePrisoner::GuardCatch(const SGCoordinates2D<unsigned short> *const kpkPrisonerPosition, 
								   const SGCoordinates2D<unsigned short> *const kpkGuardPosition) const
{
	if(*kpkPrisonerPosition == kpkGuardPosition)
	{
		return true;
	}
	return false;
}

bool CMCTSNodePrisoner::WantsToSurrender() const
{
	if((this->m_dQ / this->m_iN) < 0.1
		&& CalculateEuclideanDistance(this->m_kpkActualPosition, this->m_kpkLevel->GetExitPosition()) > 3)
	{
		return true;
	}
	return false;
}


bool CMCTSNodePrisoner::IsTerminal() const
{
	if(this->m_bLooseConditionReached || this->m_bWinConditionReached)
	{
		return true;
	}
	return false;
}

void CMCTSNodePrisoner::NewChildrenState(vector<SGCoordinates2D<unsigned short>> *const pkvNewPossiblePositions,
									     const SGCoordinates2D<unsigned short> *const kpkPrisonerPosition)
{
	SGCoordinates2D<unsigned short> newSourcePosition = SGCoordinates2D<unsigned short>();
	for(int i = 0 ; i < (int)ECMOVES::Count; i++)
	{
		newSourcePosition = *kpkPrisonerPosition;
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
		pkvNewPossiblePositions->push_back(SGCoordinates2D<unsigned short>(newSourcePosition));
	}
}

double CMCTSNodePrisoner::Simulation(const unsigned short kusNumberOfSimulations)
{
	double dDeltaValue = 0;
	if(this->m_bWinConditionReached)
	{
		return 1;
	}
	if(this->m_bLooseConditionReached)
	{
		return 0;
	}
	SGCoordinates2D<unsigned short> actualPositionCpy(this->m_kpkActualPosition);
	SGCoordinates2D<unsigned short> guardPositionCpy(this->m_kpkOpponentPosition);
	vector<SGCoordinates2D<unsigned short>> vTempStates;
	for(unsigned int i = 0; i < kusNumberOfSimulations; i++)
	{
		NewPositionCloserToTarget(&guardPositionCpy,&actualPositionCpy, this->m_kpkLevel);		
		NewChildrenState(&vTempStates, &actualPositionCpy);
		srand((unsigned int)time(0));
		int iNextActionIndex = ((rand() %  vTempStates.size()));
		actualPositionCpy = vTempStates.at(iNextActionIndex);
		if(GuardCatch(&actualPositionCpy, &guardPositionCpy))
		{
			break;
		}
		if(FinalConditionMet(&actualPositionCpy))
		{
			dDeltaValue += 1 * (kusNumberOfSimulations - i);
			break;
		}
		dDeltaValue += fmod(((double)vTempStates.size() / (CalculateEuclideanDistance(&actualPositionCpy, this->m_kpkLevel->GetExitPosition()))), 1.0);			
		vTempStates.clear();
	}
	// keep the delta value between 0 and 1
	dDeltaValue /= (double)kusNumberOfSimulations;
	return dDeltaValue;
}



