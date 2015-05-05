//////////////////////////////////////////////////////////////////////////////////////////////
// @author: Michele Ermacora
//////////////////////////////////////////////////////////////////////////////////////////////


#include "CGameField.h"
#include "CheckMemoryLeaks.h"

CGameField::CGameField(const unsigned short kusHeight, 
					   const unsigned short kusWidth,
					   const unsigned short kusNumberOfObstacles)
				:m_kusNumberOfObstacles(kusNumberOfObstacles)
{	
	this->m_pExitPosition = new SGCoordinates2D<unsigned short>();
	this->m_pGameMap = new CGMatrix<char>(kusHeight,kusWidth);
	for(unsigned int uiColoumn = 0; uiColoumn < this->m_pGameMap->m_kuiMatrixColoumnSize; uiColoumn++)
	{		
		if(uiColoumn == 0 || uiColoumn == this->m_pGameMap->m_kuiMatrixRowSize - 1)
		{
			for(unsigned int uiRow = 0; uiRow < this->m_pGameMap->m_kuiMatrixRowSize; uiRow++)
			{
				this->m_pGameMap->ChangeMatrix(uiColoumn, uiRow, this->m_kcBorderObstaclesChar);
			}
		}
		else
		{
			for(unsigned int uiRow = 1; uiRow < this->m_pGameMap->m_kuiMatrixRowSize - 1; uiRow++)
			{
				this->m_pGameMap->ChangeMatrix(uiColoumn, uiRow, ' ');
			}
			this->m_pGameMap->ChangeMatrix(uiColoumn, 0, this->m_kcBorderObstaclesChar);
			this->m_pGameMap->ChangeMatrix(uiColoumn, this->m_pGameMap->m_kuiMatrixRowSize - 1, this->m_kcBorderObstaclesChar);
		}	
	}
	AddContent();
}

CGameField::~CGameField()
{
	delete this->m_pGameMap;
	delete this->m_pExitPosition;
}


CGameField::CGameField(const CGameField* kpOldObject)
	:m_kusNumberOfObstacles(kpOldObject->m_kusNumberOfObstacles)
{
	this->m_pExitPosition = new SGCoordinates2D<unsigned short>(kpOldObject->m_pExitPosition);
	this->m_pGameMap = new CGMatrix<char>(kpOldObject->m_pGameMap);
}

void CGameField::AddContent() 
{
	int iRangeColoumn=(this->m_pGameMap->m_kuiMatrixColoumnSize - 1);
	int iRangeRow = (this->m_pGameMap->m_kuiMatrixRowSize  - 1);
	srand((unsigned int)time(0));
	//add exit
	this->m_pExitPosition->y = 1 + (rand() % (iRangeColoumn / 2));
	this->m_pExitPosition->x = 1 + (rand() % (iRangeRow / 2));
	this->m_pGameMap->ChangeMatrix(this->m_pExitPosition->y, this->m_pExitPosition->x, this->m_kcExit);	
	//add obstacles
	SGCoordinates2D<unsigned short> obstacles = SGCoordinates2D<unsigned short>();
	int iNumbOfRemainingObstacles = this->m_kusNumberOfObstacles;
	while(iNumbOfRemainingObstacles != 0)
	{
		obstacles.y = 1 + (rand() % (iRangeColoumn));
		obstacles.x = 1 + (rand() % (iRangeRow));
		if(IsPositionFree(obstacles.y, obstacles.x))
		{	
			this->m_pGameMap->ChangeMatrix(obstacles.y, obstacles.x, this->m_kcBorderObstaclesChar);	
			iNumbOfRemainingObstacles --;
		}
	}	
}


void CGameField::ClearPosition(const unsigned short kusY, 
							   const unsigned short kusX)
{
	if(kusX <= 0 || kusY <= 0 || kusX >= this->m_pGameMap->m_kuiMatrixRowSize || kusY >= this->m_pGameMap->m_kuiMatrixColoumnSize)
	{
		throw logic_error("Positions of the array outside the boundaries of the game field");
	}
	this->m_pGameMap->ChangeMatrix(kusY, kusX, ' ');
}

void CGameField::ClearPosition(const SGCoordinates2D<unsigned short>* const kpkPositionToClean)
{
	const unsigned short kusX = kpkPositionToClean->x;
	const unsigned short kusY = kpkPositionToClean->y;
	if(kusX < 0 || kusY < 0 || kusX > this->m_pGameMap->m_kuiMatrixRowSize || kusY > this->m_pGameMap->m_kuiMatrixColoumnSize)
	{
		throw logic_error("Positions of the array outside the boundaries of the game field");
	}
	this->m_pGameMap->ChangeMatrix(kusY, kusX, ' ');
}

void CGameField::ClearScreen()
{
	HANDLE                     hStdOut;
	CONSOLE_SCREEN_BUFFER_INFO csbi;
	DWORD                      count;
	DWORD                      cellCount;
	COORD                      homeCoords = { 0, 0 };

	hStdOut = GetStdHandle( STD_OUTPUT_HANDLE );
	if (hStdOut == INVALID_HANDLE_VALUE) return;

	/* Get the number of cells in the current buffer */
	if (!GetConsoleScreenBufferInfo( hStdOut, &csbi )) return;
	cellCount = csbi.dwSize.X *csbi.dwSize.Y;

	/* Fill the entire buffer with spaces */
	if (!FillConsoleOutputCharacter(
		hStdOut,
		(TCHAR) ' ',
		cellCount,
		homeCoords,
		&count
		)) return;

	/* Fill the entire buffer with the current colors and attributes */
	if (!FillConsoleOutputAttribute(
		hStdOut,
		csbi.wAttributes,
		cellCount,
		homeCoords,
		&count
		)) return;

	/* Move the cursor home */
	SetConsoleCursorPosition( hStdOut, homeCoords );
}

bool CGameField::IsPositionFree(const SGCoordinates2D<unsigned short>* const kpkPosition) const
{
	const unsigned short kusY = kpkPosition->y;
	const unsigned short kusX = kpkPosition->x;
	if(kusY > 0 && kusY < this->m_pGameMap->m_kuiMatrixColoumnSize 
		&& kusX > 0 && kusX < this->m_pGameMap->m_kuiMatrixRowSize)
	{
		if(this->m_pGameMap->GetMatrix()[kusY][kusX] != this->m_kcBorderObstaclesChar)
		{
			return true;
		}
	}
	return false;
}

bool CGameField::IsPositionFree(const unsigned short kusY,
							   const unsigned short kusX) const
{
	if(kusY > 0 && kusY < this->m_pGameMap->m_kuiMatrixColoumnSize 
		&& kusX > 0 && kusX < this->m_pGameMap->m_kuiMatrixRowSize)
	{
		if(this->m_pGameMap->GetMatrix()[kusY][kusX] != this->m_kcBorderObstaclesChar)
		{
			return true;
		}
	}
	return false;
}

void CGameField::DisplayLevel()
{
	ClearScreen();
	//Check if the exit was sobstitute by the letter of a player that passed on it
	if (IsPositionFree(this->m_pExitPosition->y, this->m_pExitPosition->x))
	{
		this->m_pGameMap->ChangeMatrix(this->m_pExitPosition->y, this->m_pExitPosition->x, this->m_kcExit);
	}
	for(unsigned int uiColoumns = 0; uiColoumns < this->m_pGameMap->m_kuiMatrixColoumnSize; uiColoumns++)
	{		
		for(unsigned int uiRow = 0; uiRow < this->m_pGameMap->m_kuiMatrixRowSize; uiRow++)
		{
			cout << this->m_pGameMap->GetMatrix()[uiColoumns][uiRow];
		}
		cout << endl;
	}
}

void CGameField::GameOver(const char* const kpkcMessage)
{
	ClearScreen();	
	cout << kpkcMessage << endl;
}

void CGameField::ModifyGameField(const unsigned short kusY,
								 const unsigned short kusX,
								 const char kcValue)
{
	this->m_pGameMap->ChangeMatrix(kusY, kusX, kcValue);
}

void CGameField::UpdateLevel(const char kcName, 
							 SGCoordinates2D<unsigned short>* const pkActualPosition)
{
	this->m_pGameMap->ChangeMatrix(pkActualPosition->y, pkActualPosition->x, kcName);
}

const char* const* CGameField::GetConstGameField() const
{
	return this->m_pGameMap->GetMatrix();
}

const SGCoordinates2D<unsigned short> *CGameField::GetExitPosition() const
{
	return this->m_pExitPosition;
}

char CGameField::GetExitLetter() const
{
	return this->m_kcExit;
}

char CGameField::GetFieldBorderLetter() const
{
	return this->m_kcBorderObstaclesChar;
}

unsigned short CGameField::GetHeight() const
{
	return this->m_pGameMap->m_kuiMatrixColoumnSize;
}

unsigned short CGameField::GetWidth() const
{
	return this->m_pGameMap->m_kuiMatrixRowSize;
}

void CGameField::DrawMCTSDebugPath(const vector<SGCoordinates2D<unsigned short>* const> *const kpkvDebugPath, 
								   const char kcDebugCharacter)
{
	for(unsigned int i = 0; i < kpkvDebugPath->size(); i++)
	{
		const SGCoordinates2D<unsigned short> * kpCoordinates = kpkvDebugPath->at(i);
		//check that the otehr player is not in the position
		if(this->m_pGameMap->GetMatrix()[kpCoordinates->y][kpCoordinates->x] == ' ')
		{
			ModifyGameField(kpCoordinates->y, kpCoordinates->x, kcDebugCharacter);
		}
	}
}

void CGameField::CleanMCTSDebugPath(const vector<SGCoordinates2D<unsigned short>* const> *const kpkvDebugPath)
{
	for(unsigned int i = 0; i < kpkvDebugPath->size(); i++)
	{
		const SGCoordinates2D<unsigned short> * kpCoordinates = kpkvDebugPath->at(i);
		ClearPosition(kpCoordinates->y, kpCoordinates->x);
	}
}



