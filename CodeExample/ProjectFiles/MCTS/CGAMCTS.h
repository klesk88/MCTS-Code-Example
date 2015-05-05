//////////////////////////////////////////////////////////////////////////////////////////////
// @author: Michele Ermacora
//
// This class contains the main algorithm for the Monte Carlo Tree Search(MCTS), used 
// for search the best move to perform by the Guard/Prisoner.
//
// MCTS is a search tree in which each node is a possible move, and a large number of random 
// simulations are used to estimate the long-term potential of each move. 
// This method has four main steps:
// - Starting at the root node of the tree, select optimal child nodes until a leaf node is reached.
// - Expand the leaf node and choose one of its children.
// - Play a simulated game starting with that node.
// - Use the results of that simulated game to update the node and its ancestors.
//
// A more exhaustive explanation of the algorithm can be found at 
// http://www.cameronius.com/cv/mcts-survey-master.pdf.
// This implementation will use the UCT formula for discover the more promising next move.
// It is a friend class of the Node class, because it needs to access the protected and private 
// methods and variables of it. 
//////////////////////////////////////////////////////////////////////////////////////////////

#ifndef _MCTS_H
#define _MCTS_H

//Set variable to TRUE for display in the level rendered
//to the console the best path at each step calculate by the MCTS
#define DEBUG_MCTS		FALSE


#include <ctime>
#include <Windows.h>
#include <limits>


#include "..\DataStructures.h"
#include "..\CGameField.h"
#include "..\MCTSNode\CGAMCTSNode.h"
#include "..\CheckMemoryLeaks.h"

using namespace std;

template <class T>
class CGAMCTS
{
public:
	CGAMCTS(const double kdConstant, 
			unsigned short kusNumberOfSimulations, 
			const float kfDelay);
	~CGAMCTS();
	//Calculate the most promising move and update the second parameter
	//passed to the method (pkActualPosition) with this new value.
	void StartMCTS(const CGameField *const kpkLevel,
				   SGCoordinates2D<unsigned short> *const pkActualPosition, 
				   const SGCoordinates2D<unsigned short> *const kpkOpponentPosition, 
				   bool* const pkbSurrender);	
	//DEBUG FUNCTIONS
	const vector<SGCoordinates2D<unsigned short>* const>* GetDebugPath() const;
private:
	//Calculate best child, at that tree level, using UCT formula
	T *BestChild(T *pNode, const double kdConstant); 
	//update the nodes using the new delta value calculated
	void Backup(CGAMCTSNode<T> *pNode,const double kdDelta);
	//Calculate delta value for the node
	double DefaultPolicy(T *pNode);
	//Add childs to the tree 
	T *Expand(T *pNodeToExpand);	
	//Check if the selected node is full expanded in which case we select the node more
	//promising using the BestChild value. If we have other possible moves we need 
	//to expand it using the Expand method
	T *TreePolicy(T *pActualNode);
	double UCTFormula(const T* const pkpParentNode, const T* const pkpChildNode, const double kdConstant); 
	//DEBUG FUNCTIONS
	void CreateDebugData(T* pPromisingNode);
private:
	//Costant used for the UCT forumla for decide if we want more exploration of the space
	//or more exploitation
	const double m_kdConstant;
	//Time given to the MCTS
	const float m_kfDelay;	
	//Number of simulations allowed for each node
	const unsigned short m_kusNumberOfSimulations;
	//DEBUG VARIABLE
	vector<SGCoordinates2D<unsigned short>* const> m_vDebugPath;
};

//IMPLEMENTATION:

template <class T>
CGAMCTS<T>::CGAMCTS(const double kdConstant, 
					const unsigned short kusNumberOfSimulations, 
					const float kfDelay)
	: m_kdConstant(kdConstant),
	  m_kfDelay(kfDelay),
	  m_kusNumberOfSimulations(kusNumberOfSimulations)
{

}

template <class T>
CGAMCTS<T>::~CGAMCTS()
{
	if(DEBUG_MCTS)
	{
		if(this->m_vDebugPath.size() != 0)
		{
			for(unsigned int i = 0; i < this->m_vDebugPath.size(); i++)
			{
				delete this->m_vDebugPath.at(i);
			}
			this->m_vDebugPath.clear();
		}
	}
}

template <class T>
void CGAMCTS<T>::StartMCTS(const CGameField *const kpkLevel,
						   SGCoordinates2D<unsigned short> *const pkActualPosition, 
						   const SGCoordinates2D<unsigned short> *const kpkOpponentPosition, 
						   bool* const pkbSurrender) 
{
	SGCoordinates2D<unsigned short> *pOpponentPositionCpy = new SGCoordinates2D<unsigned short>(kpkOpponentPosition);
	SGCoordinates2D<unsigned short> *pActualPositionCpy = new SGCoordinates2D<unsigned short>(pkActualPosition);
	SInfoForNodeInit info = SInfoForNodeInit(kpkLevel, (ECMOVES)NULL, pActualPositionCpy, pOpponentPositionCpy);
	T *pRoot = new T(info, nullptr); 
	double dDelta = 0;	
	clock_t begin = clock();
	while((((float)(clock() - begin))/CLOCKS_PER_SEC) < this->m_kfDelay)
	{
		T *pNode = TreePolicy(pRoot);
		dDelta = DefaultPolicy(pNode);
		Backup(pNode,dDelta);		
	}
	T* kpkMorePromisingChild =  BestChild(pRoot,0);
	*(pkbSurrender) = kpkMorePromisingChild->WantsToSurrender();
	*(pkActualPosition) = *(kpkMorePromisingChild->GetActualPosition());
	if(DEBUG_MCTS)
	{
		CreateDebugData(kpkMorePromisingChild);
	}
	delete pRoot;
}

template <class T>
T *CGAMCTS<T>::TreePolicy(T *pActualNode)
{
	do
	{
		if(pActualNode->CheckIfCanAddChildren())
		{
			return Expand(pActualNode);
		}
		else
		{
			pActualNode = BestChild(pActualNode, this->m_kdConstant);
		}
	}while(!pActualNode->IsTerminal());
	return pActualNode;
}

template <class T>
T *CGAMCTS<T>::Expand(T *pNode)
{
	T* pNewChild = pNode->ExpandChildren();
	if(pNewChild == nullptr)
	{
		throw logic_error("The child that was tried to expand doesn't have anymore children");
	}
	return pNewChild;
}

template <class T>
double CGAMCTS<T>::UCTFormula(const T* const pkpParentNode, const T* const pkpChildNode, const double kdConstant)
{
	return (pkpChildNode->GetQ()/(double)pkpChildNode->GetN()) + kdConstant * (sqrt((2*log((double)pkpParentNode->GetN()))/(double)pkpChildNode->GetN()));
}

template <class T>
T *CGAMCTS<T>::BestChild(T *pNode, const double kdConstant)
{
	double dTempValue = -1;
	double dBestChildValue = -1;
	T *pBestChild = nullptr;
	int iChildrenSize = pNode->GetExpandedChildrenSize();
	for(int i = 0; i < iChildrenSize; i++)
	{
		T *children = pNode->GetExpandedChildren(i);
		dTempValue = UCTFormula(pNode, children, kdConstant);
		if(dTempValue > dBestChildValue)
		{
			dBestChildValue = dTempValue;
			pBestChild = children;
		}
		else
		{
			if(dTempValue == dBestChildValue)
			{
				srand((unsigned int)time(0));
				int iRange = ((rand() %  100) + 1);
				if(iRange > 50)
				{
					pBestChild = children;
				}
			}
		}
	}
	return pBestChild;
}

template <class T>
double CGAMCTS<T>::DefaultPolicy(T *pNode)
{
	if(pNode != nullptr)
	{
		return pNode->Simulation(this->m_kusNumberOfSimulations);
	}
	else
	{
		throw logic_error("Problem with the expansion phase");
	}
}

template <class T>
void CGAMCTS<T>::Backup(CGAMCTSNode<T> *pNode,const double kdDelta)
{
	while(pNode != nullptr)
	{
		pNode->IncreaseN();
		pNode->SetQ(kdDelta);
		pNode = pNode->GetParent();
	}
}

template <class T>
void CGAMCTS<T>::CreateDebugData(T *pPromisingNode)
{
	//delete the elements inside the debug path, if there are any
	if(this->m_vDebugPath.size() != 0)
	{
		for(unsigned int i = 0; i < this->m_vDebugPath.size(); i++)
		{
			delete this->m_vDebugPath.at(i);
		}
		this->m_vDebugPath.clear();
	}
	//add the coordinates of the best nodes to the debug list
	while(pPromisingNode->GetExpandedChildrenSize() != 0)
	{
		pPromisingNode = BestChild(pPromisingNode,0);
		this->m_vDebugPath.push_back(new SGCoordinates2D<unsigned short>(pPromisingNode->GetActualPosition()));
	}
	this->m_vDebugPath.push_back(new SGCoordinates2D<unsigned short>(pPromisingNode->GetActualPosition()));
}

template <class T>
const vector<SGCoordinates2D<unsigned short>* const> *CGAMCTS<T>::GetDebugPath() const
{
	return &(this->m_vDebugPath);
}

#endif