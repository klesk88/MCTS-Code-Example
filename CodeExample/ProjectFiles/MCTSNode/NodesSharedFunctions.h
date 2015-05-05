//////////////////////////////////////////////////////////////////////////////////////////////
// @author: Michele Ermacora
//
// Class that share the common functions between nodes that are not part of a MCTS node logic.
//////////////////////////////////////////////////////////////////////////////////////////////

#ifndef NODES_SHARED_FUNCTIONS_H
#define NODES_SHARED_FUNCTIONS_H

#include <vector>

#include "CGAMCTSNode.h"
#include "..\CGameField.h"
#include "..\SharedFunctions.h"


//Function that updates the first pointer passed with a new position more closer
//to the target position.
void NewPositionCloserToTarget(SGCoordinates2D<unsigned short> *const pkSourcePosition, 
							   const SGCoordinates2D<unsigned short> *const kpkTargetPosition,
							   const CGameField *const kpkLevel);

#endif