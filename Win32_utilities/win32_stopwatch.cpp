// win32_stopwatch.cpp
//

#pragma once

#include "stdafx.h"
#include <iostream>
#include <iomanip>
#include "assert.h"
#include "windows.h"
#include "win32_stopwatch.h"

int Stopwatch::m_iLevel = 0;

void Stopwatch::Start( )
{
	++m_iLevel; 
	m_hrtimer.Start( );
}

void Stopwatch::Stop( wstring const wstr )
{
	assert( m_iLevel > 0 );  // no Stop without Start

	m_hrtimer.Stop( );
	DWORD dwMicroseconds = m_hrtimer.Get( );
	DWORD dwMilliseconds = dwMicroseconds / 1000;
	DWORD dwFraction     = dwMicroseconds - dwMilliseconds * 1000;
	--m_iLevel;
	for ( int i = 0; i < m_iLevel; ++i )
		wcout << L"      ";
	wcout << setw(30) << left << wstr;
	wcout << setw(6) << right << dwMilliseconds << L"." << dwFraction << L" ms" << endl;
}
