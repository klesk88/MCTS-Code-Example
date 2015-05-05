//////////////////////////////////////////////////////////////////////////////////////////////
// @author: Michele Ermacora
//////////////////////////////////////////////////////////////////////////////////////////////

#include "NodesSharedFunctions.h"
#include "..\CheckMemoryLeaks.h"


void NewPositionCloserToTarget(SGCoordinates2D<unsigned short> *const pkSourcePosition,
							   const SGCoordinates2D<unsigned short> *const kpkTargetPosition,
							   const CGameField *const kpkLevel)
{		
	double dDistanceGuardTheft = DBL_MAX;
	SGCoordinates2D<unsigned short> newSourcePosition = SGCoordinates2D<unsigned short>();
	SGCoordinates2D<unsigned short> tempFinalPosition = SGCoordinates2D<unsigned short>();
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
		if(!kpkLevel->IsPositionFree(&(newSourcePosition)))
		{
			continue;
		}
		double dNewDistance = CalculateEuclideanDistance(&newSourcePosition, kpkTargetPosition);
		if(dNewDistance < dDistanceGuardTheft)
		{
			dDistanceGuardTheft = dNewDistance;
			tempFinalPosition = newSourcePosition;
		}
		else
		{
			if(dNewDistance == dDistanceGuardTheft)
			{
				srand((unsigned int)time(0));
				int iRange = ((rand() %  100) + 1);
				if(iRange > 50)
				{
					tempFinalPosition = newSourcePosition;
				}
			}
		}
	}				
	*pkSourcePosition = tempFinalPosition;
}


