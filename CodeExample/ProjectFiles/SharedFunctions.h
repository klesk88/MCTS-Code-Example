//////////////////////////////////////////////////////////////////////////////////////////////
// @author: Michele Ermacora
// 
// Class containing shared functions use throught the code
//////////////////////////////////////////////////////////////////////////////////////////////

#ifndef SHARED_FUNCTIONS_H
#define SHARED_FUNCTIONS_H

#include <cmath>

#include "DataStructures.h"
#include "CheckMemoryLeaks.h"

//Calculate Euclidean distance in 2D
inline double CalculateEuclideanDistance(const SGCoordinates2D<unsigned short> *const kpkFirstPoint, 
									  const SGCoordinates2D<unsigned short> *const kpkSecondPoint)
{
	return sqrt(pow(kpkFirstPoint->y - kpkSecondPoint->y, 2) + pow(kpkFirstPoint->x - kpkSecondPoint->x, 2));

}	

#endif