// ConsoleApplication1.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "windows.h"
#include "gridNeighbor.h"
// #include "food.h"

using namespace std;

int iSum;

int _tmain(int argc, _TCHAR* argv[])
{
	const int COUNT = 1000L;
    
    int iThreshold  =  10;
	DWORD milliSeconds = GetTickCount();

	for ( long l = 0; l < COUNT; l++ )
    {
        iSum = 0;
	    GridPoint gpRun = GridPoint::GRID_ORIGIN;
	    do
	    {
    		int iFood = 100;
		    if ( iFood > iThreshold )
		    {
     		    NEIGHBORHOOD_ITERATOR iter( gpRun );
			    do
			    {
                    GridPoint gp = iter.Get();
                    iSum++; 
			    } while ( iter.Next( ) );
		    }

	    } while (++gpRun != GridPoint::GRID_ORIGIN);
    }
    
	milliSeconds = GetTickCount() - milliSeconds;
    cout << milliSeconds << endl;
    cout << iSum << endl;
    getchar();
	return 0;
}
