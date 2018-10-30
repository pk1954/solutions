// win32_rightColumn.cpp
//

#pragma once

#include <iomanip>
#include "win32_gridPointShape.h"

class RightColumn : public GridPointShape
{
public:
	using GridPointShape::GridPointShape;

	void FillBuffer( GridPoint const gp )
	{
		MEM_INDEX const memSize   = m_pCore->GetMemSize( gp );  
		MEM_INDEX const memFilled = m_pCore->GetMemUsed( gp ); 
        
		m_wBuffer << L"  Mem " << memFilled << L"/" << memSize << endl;

		for	( MEM_INDEX mem = 0; mem < memFilled; ++mem )
		{
			m_wBuffer << setw( 10 ) << m_pCore->GetMemEntry( gp, mem ) << endl;
		}
	}
};
