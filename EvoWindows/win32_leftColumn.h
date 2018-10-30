// win32_leftColumn.cpp
//

#pragma once

#include <iomanip>     
#include "plannedActivity.h"
#include "win32_gridPointShape.h"

class LeftColumn : public GridPointShape
{
public:
	using GridPointShape::GridPointShape;

	void FillBuffer( GridPoint const gp )
	{
		m_wBuffer << gp                                                                << endl;
		m_wBuffer << L"ID: " << setw( 5 ) << m_pCore->GetId( gp )                      << endl;
		m_wBuffer << L"En: " << setw( 5 ) << m_pCore->GetEnergy( gp )                  << endl;
		m_wBuffer << L"Age:" << setw( 5 ) << m_pCore->GetAge( gp )                     << endl;
		m_wBuffer << L"Or: " << setw( 5 ) << GetOriginName( m_pCore->GetOrigin( gp ) ) << endl;

		PlannedActivity plan = m_pCore->GetPlan( );
		if ( plan.IsValid( ) )
		{
			if ( (gp == plan.GetActor( )) || (gp == plan.GetPartner( )) )
			{
				m_wBuffer << (( gp == plan.GetActor( ) ) ? L"** ACTOR **" : L"** PARTNER **") << endl;
				m_wBuffer << L"** "  << GetActionTypeName( plan.GetActionType( ) ) << L" **"  << endl;
				m_wBuffer << L"BaseCons: "  << plan.GetBaseConsumption( )                     << endl;
			}
		}
	}
};
