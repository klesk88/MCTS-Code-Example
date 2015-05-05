//////////////////////////////////////////////////////////////////////////////////////////////
// @author: Michele Ermacora
//
// This class is use for check if there are memory leaks inside the program.
// Place the function _CrtDumpMemoryLeaks(); in any point of the program to
// check the presence of memory leaks (one is already placed at the end of the main).
// For enable the check of memory leaks set TRUE the macro
// "ENABLE_CHECK_MEMORY_LEAKS". 
// Remember to include this header to all the files in which is needed the information
// of which class is creating the memory leak.
//////////////////////////////////////////////////////////////////////////////////////////////

#ifndef CHECK_MEMORY_LEAKS_H
#define CHECK_MEMORY_LEAKS_H

#define _CRTDBG_MAP_ALLOC
#define ENABLE_CHECK_MEMORY_LEAKS			FALSE

#include <stdlib.h>
#include <crtdbg.h>

#if ENABLE_CHECK_MEMORY_LEAKS
	#ifndef DBG_NEW      
		#define DBG_NEW new ( _NORMAL_BLOCK , __FILE__ , __LINE__ )    
		#define new DBG_NEW  
	#endif
#endif

#endif


