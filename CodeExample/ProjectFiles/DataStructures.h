//////////////////////////////////////////////////////////////////////////////////////////////
// @author: Michele Ermacora
//
// This class implement the main data structures use troughout the code. 
// It contains a SGCoordinates2D struct used for store the x, y coordinates of a cell inside
// the level (represented as a matrix of characters).
// It also contains a class CGMatrix used for store the matrix of char representing the level,
// as well as containing the methods for accessing the matrix and modifying it.
//////////////////////////////////////////////////////////////////////////////////////////////

#ifndef DATASTRUCTURES_H
#define DATASTRUCTURES_H

#include "CheckMemoryLeaks.h"


template <class T>
struct SGCoordinates2D
{
public:
    SGCoordinates2D(T y=0, T x=0) 
        : x(x), y(y)
    {
    }
	SGCoordinates2D(const SGCoordinates2D *const kpkStructToCopy) 
    {
		x = kpkStructToCopy->x;
		y = kpkStructToCopy->y;
    }
	SGCoordinates2D& operator=(const SGCoordinates2D& kSecondStruct)
    {
        x = kSecondStruct.x;
        y = kSecondStruct.y;
        return *this;
    }
    SGCoordinates2D operator+(const SGCoordinates2D& kSecondStruct) const
    {
        return SGCoordinates2D(kSecondStruct.x + x, kSecondStruct.y + y);
    }
    bool operator==(const SGCoordinates2D& kSecondStruct) const
    {
        return (x == kSecondStruct.x && y == kSecondStruct.y);
    }
	bool operator==(const SGCoordinates2D* const kpkSecondStruct) const
    {
        return (x == kpkSecondStruct->x && y == kpkSecondStruct->y);
    }
	bool operator!=(const SGCoordinates2D* const kpkSecondStruct) const
    {
        return (x != kpkSecondStruct->x && y != kpkSecondStruct->y);
    }
public:
	T y, x;
};



template <class U>
class CGMatrix
{
public:
	CGMatrix(const int kiColoumnSize, 
			 const int kiRowSize);
	~CGMatrix();
	CGMatrix(const CGMatrix<U> *kpOldObject);
	const U* const* GetMatrix() const;
	void ChangeMatrix(int iColoumn, int iRow, U value);
public:
	const unsigned int m_kuiMatrixColoumnSize;
	const unsigned int m_kuiMatrixRowSize;
private:
	U **m_ppMatrix;
};


template<class U>
CGMatrix<U>::CGMatrix(const int kiColoumnSize, 
					  const int kiRowSize)
	:m_kuiMatrixColoumnSize(kiColoumnSize),
	m_kuiMatrixRowSize(kiRowSize)
{
	this->m_ppMatrix = new U*[kiColoumnSize];
	for(int iColoumn = 0; iColoumn < kiColoumnSize; iColoumn++)
	{
		this->m_ppMatrix[iColoumn] = new U[kiRowSize]; 
		
		for(int iRow = 0; iRow < kiRowSize; iRow++)
		{
			this->m_ppMatrix[iColoumn][iRow] = 0;
		}
		
	}
}

template<class U>
CGMatrix<U>::~CGMatrix()
{
	for(unsigned int uiColoumn = 0; uiColoumn < this->m_kuiMatrixColoumnSize; uiColoumn++)
	{
		delete[] this->m_ppMatrix[uiColoumn];
	}
	delete[] this->m_ppMatrix ;
}

template<class U>
CGMatrix<U>::CGMatrix(const CGMatrix<U> *kpOldObject)
	:m_kuiMatrixColoumnSize(kpOldObject->m_kuiMatrixColoumnSize),
	m_kuiMatrixRowSize(kpOldObject->m_kuiMatrixRowSize)
{
	this->m_ppMatrix = new U*[kpOldObject->m_kuiMatrixColoumnSize];
	for(unsigned int uiColoumn = 0; uiColoumn < kpOldObject->m_kuiMatrixColoumnSize; uiColoumn++)
	{
		this->m_ppMatrix[uiColoumn] = new U[kpOldObject->m_kuiMatrixRowSize]; 
		for(unsigned int uiRow = 0; uiRow < kpOldObject->m_kuiMatrixRowSize; uiRow++)
		{
			this->m_ppMatrix[uiColoumn][uiRow] = kpOldObject->m_ppMatrix[uiColoumn][uiRow];
		}
	}

}

template<class U>
const U* const* CGMatrix<U>::GetMatrix() const
{
	return this->m_ppMatrix;
}

template<class U>
void CGMatrix<U>::ChangeMatrix(int iColoumn, int iRow, U value)
{
	this->m_ppMatrix[iColoumn][iRow] = value;
}

#endif