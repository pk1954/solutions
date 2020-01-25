// win32_stopwatch.cpp
//
// Toolbox/win32_utilities

#pragma once

#include "stdafx.h"
#include <sstream> 
#include <iostream>
#include <iomanip>
#include "assert.h"
#include "util.h"
#include "windows.h"
#include "win32_stopwatch.h"

int Stopwatch::m_iLevel = 0;

void Stopwatch::Start( )
{
	++m_iLevel; 
	m_hrtimer.Start( );
}

void Stopwatch::Stop( std::wstring const wstr )
{
	assert( m_iLevel > 0 );  // no Stop without Start

	m_hrtimer.Stop( );
	microseconds microSecs = m_hrtimer.GetDuration( );
	float        millisecs = CastToFloat( microSecs.count() ) / 1000.0f;
	--m_iLevel;
	for ( int i = 0; i < m_iLevel; ++i )
		std::wcout << L"      ";
	std::wcout << std::setw(30) << std::left << wstr;
	std::wcout << std::setw( 6) << std::right;
	std::wcout << std::fixed << std::setprecision(2) << millisecs;
	std::wcout << L" ms" << std::endl;
}
