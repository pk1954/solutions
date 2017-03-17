/// HistoryTest.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include "HistorySystem.h"

using namespace std;

static int test( int iMax )
{
/*
    History * hHist = History::GetInstance( (char *)&lGeneration, sizeof(lGeneration), 500 );
	int iSize = hHist->GetMemorySize();

	for ( int i = 1; i < iSize + 1000; i++ )
	{
		hHist->Add();
	}

	for ( int i = 1; i < 29; i++ ) 
	{
		hHist->Backward();
	}

	hHist->Dump();

	cout << "Discard: " << endl;
	hHist->Discard();
	hHist->Dump();

	for ( int i = 1; i < 100; i++ ) 
	{
		hHist->Add();
		hHist->Dump();
	}

*/	
	return 0;
}

int _tmain(int argc, _TCHAR* argv[])
{

	int iRes = test( 300000 );

	return 0;
}
