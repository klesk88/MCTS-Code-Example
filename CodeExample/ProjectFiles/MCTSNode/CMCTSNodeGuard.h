//////////////////////////////////////////////////////////////////////////////////////////////
// @author: Michele Ermacora
//
// This class specialize the MCTSNode class. During the simulation step the Guard will randomly 
// choose between the two moves that mostly decrease the air distance between him and the Prisoner. 
// The Prisoner, instead, will always select the best move that mostly decrese the air distance 
// between him and the exit.
// If the node is a leaf where the Guard catch the Prisoner, the delta value return will be 1,
// otherwise, if the node is a leaf in which the Prisoner manage to escape, the delta value
// returned will be 0.
// If the node is not a leaf a number of simulation steps, decided by the suer, will be performed.
// If the Prisoner escape, the simulation step will break, and the actual delta value returned.
// If the Guard catch the Prisoner the simulation will break and, to the delta value, will be
// add 1 multiply the number of simulation steps remaining. 
// If the distance between the Guard and the Prisoner is sqrt(2), meaning the Guard is in a 
// diagonal position compared to the Prisoner, the delta value will increase by 0.7
// as he has a chance to cacth the Prisoner in the next step.
// Otherwise to the delta value wil be added 0.3.
//////////////////////////////////////////////////////////////////////////////////////////////

#ifndef MCTS_NODE_GUARD_H
#define MCTS_NODE_GUARD_H

#include "CGAMCTSNode.h"
#include "NodesSharedFunctions.h"

class CMCTSNodeGuard : public CGAMCTSNode<CMCTSNodeGuard>
{
public:
	CMCTSNodeGuard(SInfoForNodeInit info, 
				   CMCTSNodeGuard *const pkParent);
	~CMCTSNodeGuard();
private:
	template<typename U>friend class CGAMCTS; 
private:
	CMCTSNodeGuard* ExpandChildren();
	bool WantsToSurrender() const;
	bool IsTerminal() const;
	bool PrisonerEscaped(const SGCoordinates2D<unsigned short> *const kpkPrisonerPosition) const;
	bool PrisonerCatched(const SGCoordinates2D<unsigned short> *const kpkGuardPosition, 
					     const SGCoordinates2D<unsigned short> *const kpkPrisonerPosition) const;
	//Perform the simulation step of the MCTS. Explained in detail at the top
	//of this header
	double Simulation(const unsigned short kusNumberOfSimulations);
	//calculate, randomly, which of the two best moves (the ones that decrease the msot
	//the air distance between guard and prisoner) select. It stores the result in the 
	//pkGuardPosition variable
	void NewRndPositionCloserToTarget(SGCoordinates2D<unsigned short> *const pkGuardPosition, 
							   const SGCoordinates2D<unsigned short> *const kpkPrisonerPosition);
private:
	//node distance between Guard and Prisoner
	double m_dDistanceFromPrisoner;
	bool m_bWinConditionReached;
	bool m_bLooseConditionReached;
};

#endif