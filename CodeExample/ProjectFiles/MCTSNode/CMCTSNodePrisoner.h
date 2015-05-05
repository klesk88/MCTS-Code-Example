//////////////////////////////////////////////////////////////////////////////////////////////
// @author: Michele Ermacora
//
// This class specialize the CGAMCTSNode class. During the Simulation step, if the node is a
// leaf in which the Prisoner manage to escape, the delta value will be one, otherwise,
// if he gets catched by the guard, his delta value will be 0. If the node is not a leaf, 
// the simulation step will be performed a number of times decided by the user.
// For each step the Guard will select the move that decrease the most
// the air distance between him and the Prisoner.
// The Prisoner instead, will randomly select a movement between the ones available.
// If he succeed to escape the simulation will stop and the delta value will be increase by 1
// multiply by the number of simulations to perform left. 
// If the Guard catch the Prisoner, the simulation will finish and the actual delta value, 
// calculated until that moment, used. 
// Otherwise the delta value will increase depending on how much promising is
// the actual movement (how much far the Prisoner is from the exit, and how many collisions 
// he has with the obstacles).
// This last step is inspired by the paper of Munir Naveed, Diane Kitchin, Andrew Crampton : 
// "Monte-Carlo Planning for Pathfinding in Real-Time Strategy Games", which can be found at 
// http://eprints.hud.ac.uk/9242/1/plansig2010.pdf. 
/////////////////////////////////////////////////////////////////////////////////////////////

#ifndef MCTS_NODE_THIEF_H
#define MCTS_NODE_THIEF_H

#include "CGAMCTSNode.h"
#include "NodesSharedFunctions.h"

class CMCTSNodePrisoner : public CGAMCTSNode<CMCTSNodePrisoner>
{
public:
	CMCTSNodePrisoner(SInfoForNodeInit info, 
					  CMCTSNodePrisoner *const pkParent);
	~CMCTSNodePrisoner();
private:
	template<typename U>friend class CGAMCTS; 
private:
	CMCTSNodePrisoner* ExpandChildren();
	bool FinalConditionMet(const SGCoordinates2D<unsigned short> *const kpkPrisonerPosition) const;
	bool GuardCatch(const SGCoordinates2D<unsigned short> *const kpkPrisonerPosition,
				    const SGCoordinates2D<unsigned short> *const kpkGuardPosition) const;
	bool WantsToSurrender() const;
	bool IsTerminal() const;
	//Perform the simulation step of the MCTS. Explained in detail at the top
	//of this header
	double Simulation(const unsigned short kusNumberOfSimulations);
	//This method will modify the vector pass as first argument inserting in it 
	//all the possible moves that the Prisoner can perform from the actual position
	//it has.
	void NewChildrenState(vector<SGCoordinates2D<unsigned short>> *const pkvNewPossiblePositions,
						  const SGCoordinates2D<unsigned short> *const kpkPrisonerPosition);
private:
	bool m_bWinConditionReached;
	bool m_bLooseConditionReached;
};

#endif