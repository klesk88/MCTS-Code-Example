//////////////////////////////////////////////////////////////////////////////////////////////
// @author: Michele Ermacora
//////////////////////////////////////////////////////////////////////////////////////////////

#include "CGame.h"
#include "CheckMemoryLeaks.h"

CGame::CGame()
{
	

}

CGame::~CGame()
{
	delete this->m_pGameMap;
	delete this->m_pGuard;
	delete this->m_pPrisoner;
}

void CGame::Init(const unsigned short kusHeight, 
				 const unsigned short kusWidth,
				 const unsigned short kusNumberOfObstacles) 

{
	this->m_pGameMap = new CGameField(kusHeight, kusWidth, kusNumberOfObstacles);
	this->m_pGuard = new CGuard('G', this->m_pGameMap);
	this->m_pPrisoner = new CPrisoner('P', this->m_pGameMap);	
	this->m_pGuard->AddPlayerToLevel(this->m_pGameMap, this->m_pGameMap->GetExitPosition(), 3, 5);
	this->m_pPrisoner->AddPlayerToLevel(this->m_pGameMap, this->m_pGuard->GetActualPosition(), 4, 6);
	this->m_pGameMap->DisplayLevel();
}

bool CGame::ExitGame()
{
	if (*(this->m_pGuard->GetActualPosition()) == this->m_pPrisoner->GetActualPosition())
	{
		this->m_pGameMap->GameOver("PRISONER CAUGHT");
		return true;
	}
	if(*(this->m_pPrisoner->GetActualPosition()) == this->m_pGameMap->GetExitPosition())
	{
		this->m_pGameMap->GameOver("PRISONER ESCAPED");
		return true;
	}			
	if(this->m_pGuard->GetSurrender())
	{
		this->m_pGameMap->GameOver("GUARD SURRENDER");
		return true;
	}
	if(this->m_pPrisoner->GetSurrender())
	{
		this->m_pGameMap->GameOver("PRISONER SURRENDER");
		return true;
	}
	return false;
}

void CGame::GameLoop()
{
	
	SGCoordinates2D<unsigned short> actualPositionGuard = SGCoordinates2D<unsigned short>();
	SGCoordinates2D<unsigned short> actualPositionPrisoner = SGCoordinates2D<unsigned short>();
	while(true)
	{
		clock_t begin = clock();
		//wait one sec or the console is getting refresh too fast
		while(clock() < begin + 1000)
		{
			
		}
		if(DEBUG_MCTS)
		{
			this->m_pGameMap->CleanMCTSDebugPath(this->m_pGuard->GetDebugPath());
			this->m_pGameMap->CleanMCTSDebugPath(this->m_pPrisoner->GetDebugPath());
		}
		actualPositionGuard = *(this->m_pGuard->GetActualPosition());
		actualPositionPrisoner = *(this->m_pPrisoner->GetActualPosition());
		this->m_pPrisoner->Update(&actualPositionGuard);
		this->m_pGuard->Update(&actualPositionPrisoner);
		if(ExitGame())
		{
			break;
		}
		this->m_pGameMap->ClearPosition(this->m_pGuard->GetOldPosition());
		this->m_pGameMap->ClearPosition(this->m_pPrisoner->GetOldPosition());
		this->m_pGameMap->UpdateLevel(*(this->m_pPrisoner->GetName()), this->m_pPrisoner->GetActualPosition());
		this->m_pGameMap->UpdateLevel(*(this->m_pGuard->GetName()), this->m_pGuard->GetActualPosition());
		if(DEBUG_MCTS)
		{
			this->m_pGameMap->DrawMCTSDebugPath(this->m_pGuard->GetDebugPath(), '+');
			this->m_pGameMap->DrawMCTSDebugPath(this->m_pPrisoner->GetDebugPath(), '.');
		}
		this->m_pGameMap->DisplayLevel();
	}
}

