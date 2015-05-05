//////////////////////////////////////////////////////////////////////////////////////////////
// @author: Michele Ermacora
//
// This class contains the implementation of the nodes used by the MCTS. 
// All it's members are private or protected, as only the CGMCTS class must access them. That's 
// why this class is friend with the CGMCTS class.
// It also contains the enum SMovementsAllowed represeting the different moves the Guard and 
// Prisoner can select, as well as a struct that will be use for contain the data used for 
// the initialization of the nodes.
//////////////////////////////////////////////////////////////////////////////////////////////

#ifndef MCTSNODE_H
#define MCTSNODE_H

#include<list>
#include<unordered_set>
#include<vector>

#include "..\DataStructures.h"
#include "..\CGameField.h"
#include "..\CheckMemoryLeaks.h"

using namespace std;

//moves that the player can perform
enum class ECMOVES : char{DOWN = 0, UP, RIGHT, LEFT, Count};

//struct used to store informations to pass to the constructor of the new node
struct SInfoForNodeInit
{
public:
	SInfoForNodeInit(const CGameField *const pLevel,
					 const ECMOVES keMoveSelected,
					 const SGCoordinates2D<unsigned short> *const kpkActualPosition,
					 const SGCoordinates2D<unsigned short> *const kpkOpponentPosition)
		:m_kpkLevel(pLevel),
		 m_keMoveSelected(keMoveSelected),
		 m_kpkActualPosition(kpkActualPosition),
		 m_kpkOpponentPosition(kpkOpponentPosition)
	{

	};
public:
	const CGameField *const m_kpkLevel;
	const ECMOVES m_keMoveSelected;
	const SGCoordinates2D<unsigned short> *const m_kpkActualPosition;
	const SGCoordinates2D<unsigned short> *const m_kpkOpponentPosition;
};

template <class T>
class CGAMCTSNode
{
public:
	CGAMCTSNode(SInfoForNodeInit info,
				T* const pkParent);	
	~CGAMCTSNode();
protected:
	//Add children to the node
	virtual	T* ExpandChildren() = 0;
	virtual bool WantsToSurrender() const = 0;
	//Check if the  node is a terminal node (reached the final condition
	//of loose or win)
	virtual bool IsTerminal() const = 0;
	virtual double Simulation(const unsigned short kusNumberOfSimulations) = 0;
protected: 
	//Create the new child and add it to the vector containing them
	void AddChild(const SGCoordinates2D<unsigned short>* const kpkOpponentPosition);
	//Check if is possible to add more children to the node, or if is full expanded
	bool CheckIfCanAddChildren();
	void IncreaseN();
	void SetQ(const double kdDelta);	
	T *GetExpandedChildren(const int kiIndex) const;
	int GetExpandedChildrenSize() const;
	int GetN() const;
	T *GetParent() const;
	double GetQ() const;
	const SGCoordinates2D<unsigned short> *GetActualPosition() const;
	const SGCoordinates2D<unsigned short> *GetOpponentPosition() const;
private:
	template<typename U>friend class CGAMCTS; 
protected:
	T *const m_pkParent;
	//Enum storing the position performed by this node. Make more readable what is happening
	//inside the node without have to check the parent
	const ECMOVES m_kMoveSelected;
	//Vector containing the children of the node
	vector<T *const> m_vChildrensExpanded;
	//Reward score of the node
	double m_dQ;	
	//Number of times the node was visited
	int m_iN;	
	//Class contatining informations about the game field
	const CGameField *const m_kpkLevel;
	const SGCoordinates2D<unsigned short> *const m_kpkActualPosition;
	const SGCoordinates2D<unsigned short> *const m_kpkOpponentPosition;
	//varaible for keep track of which positions to expand. It stores the enum of the new position plus
	//the new position cell (x,y) value 
	pair<ECMOVES, SGCoordinates2D<unsigned short>> m_pairChildToAdd;
	//remember which children are already expanded for fast retrieval
	unordered_set<ECMOVES> m_MovesAlreadyExpanded;
};

//IMPLEMENTATION: 

template <class T>
CGAMCTSNode<T>::CGAMCTSNode(SInfoForNodeInit info, 
							T* const pkParent)
		:m_kMoveSelected(info.m_keMoveSelected),
		 m_kpkLevel(info.m_kpkLevel),
		 m_kpkActualPosition(info.m_kpkActualPosition),
		 m_kpkOpponentPosition(info.m_kpkOpponentPosition),
		 m_pkParent(pkParent)
{
	this->m_pairChildToAdd = make_pair((ECMOVES)NULL, NULL);
	this->m_iN = 0;
	this->m_dQ = 0;
}

template <class T>
CGAMCTSNode<T>::~CGAMCTSNode()
{
	for(unsigned int i = 0; i < this->m_vChildrensExpanded.size(); i++)
	{
		delete this->m_vChildrensExpanded.at(i);
	}
	this->m_vChildrensExpanded.clear();
	delete this->m_kpkOpponentPosition;
	delete this->m_kpkActualPosition;	
}

template <class T>
bool CGAMCTSNode<T>::CheckIfCanAddChildren()
{
	if(this->m_MovesAlreadyExpanded.size() != (short int)(ECMOVES::Count) && !IsTerminal())
	{		
		const int kiY = this->m_kpkActualPosition->y;
		const int kiX = this->m_kpkActualPosition->x;
		for(unsigned short i = 0; i < (unsigned short)ECMOVES::Count; i++)
		{
			ECMOVES eSelectedMove = (ECMOVES)i;
			unordered_set<ECMOVES>::const_iterator pkIt = this->m_MovesAlreadyExpanded.find(eSelectedMove);
			//check if the child was not alrady expanded previously 
			if(this->m_MovesAlreadyExpanded.count(eSelectedMove) > 0)
			{
				continue;
			}
			m_MovesAlreadyExpanded.insert((ECMOVES)i);
			if(eSelectedMove == ECMOVES::DOWN)
			{
				if(this->m_kpkLevel->IsPositionFree(kiY + 1, kiX))
				{
					this->m_pairChildToAdd = make_pair(eSelectedMove, SGCoordinates2D<unsigned short>(kiY + 1, kiX));
					return true;	
				}
				continue;
			}
			if(eSelectedMove == ECMOVES::UP)
			{
				if(this->m_kpkLevel->IsPositionFree(kiY - 1, kiX))
				{
					this->m_pairChildToAdd = make_pair(eSelectedMove, SGCoordinates2D<unsigned short>(kiY - 1, kiX));
					return true;
				}
				continue;
			}
			if(eSelectedMove == ECMOVES::LEFT)
			{
				if(this->m_kpkLevel->IsPositionFree(kiY, kiX - 1))
				{
					this->m_pairChildToAdd = make_pair(eSelectedMove, SGCoordinates2D<unsigned short>(kiY, kiX - 1));
					return true;
				}
				continue;
			}
			if(eSelectedMove == ECMOVES::RIGHT)
			{
				if(this->m_kpkLevel->IsPositionFree(kiY, kiX + 1))
				{
					this->m_pairChildToAdd = make_pair(eSelectedMove, SGCoordinates2D<unsigned short>(kiY, kiX + 1));
					return true;
				}
				continue;
			}
		}	
	}
	return false;
}

template <class T>
void CGAMCTSNode<T>::AddChild(const SGCoordinates2D<unsigned short>* const kpkOpponentPosition)
{	
	
	SGCoordinates2D<unsigned short> *pNewPosition = new SGCoordinates2D<unsigned short>(this->m_pairChildToAdd.second);
	SGCoordinates2D<unsigned short> *pOpponentPositionCpy = new SGCoordinates2D<unsigned short>(kpkOpponentPosition);
	SInfoForNodeInit info = SInfoForNodeInit(this->m_kpkLevel, this->m_pairChildToAdd.first, pNewPosition, pOpponentPositionCpy);
	this->m_vChildrensExpanded.push_back(new T(info, (T*)this));
}


template <class T>
void CGAMCTSNode<T>::IncreaseN()
{
	this->m_iN++;
}

template <class T>
void CGAMCTSNode<T>::SetQ(const double kdDelta)
{
	this->m_dQ += kdDelta;
}

template <class T>
const SGCoordinates2D<unsigned short> *CGAMCTSNode<T>::GetActualPosition() const
{
	return this->m_kpkActualPosition;
}

template <class T>
T* CGAMCTSNode<T>::GetExpandedChildren(const int kiIndex) const
{
	if(this->m_vChildrensExpanded.size() != 0)
	{
		T *pChild = (this->m_vChildrensExpanded.at(kiIndex));
		return pChild;
	}
	else
	{
		throw logic_error("Try to expand children when the size of the already expanded children is 0. Problem with the TreePolicy method");
	}
}

template <class T>
int CGAMCTSNode<T>::GetExpandedChildrenSize() const
{
	return this->m_vChildrensExpanded.size();
}

template <class T>
int CGAMCTSNode<T>::GetN() const
{
	return this->m_iN;
}

template <class T>
const SGCoordinates2D<unsigned short> *CGAMCTSNode<T>::GetOpponentPosition() const
{
	return this->m_kpkOpponentPosition;
}

template <class T>
T *CGAMCTSNode<T>::GetParent() const
{
	return this->m_pkParent;
}

template <class T>
double CGAMCTSNode<T>::GetQ() const
{
	return this->m_dQ;
}

#endif
