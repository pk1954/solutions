// win32_leftColumn.cpp
//

#pragma once

#include <iomanip>     
#include "plannedActivity.h"
#include "win32_gridPointShape.h"

class GridPointCoordShape : public GridPointShape
{
public:
	GridPointCoordShape
	( 
		Shape       * const pParent,
		TextDisplay &       textDisplay
	) :
		GridPointShape( pParent, textDisplay )
	{}

	void FillBuffer( GridPoint const gp )
	{
		m_textDisplay.Buffer() << gp;
	}

private:
};


class LeftColumn : public GridPointShape
{
public:
	using GridPointShape::GridPointShape;
	
	void FillBuffer( GridPoint const gp )
	{
		wostringstream & buffer = m_textDisplay.Buffer();
		EvolutionCore  & core   = m_textDisplay.Core();

//		buffer << gp                                                            << endl;
		buffer << L"ID: " << setw( 5 ) << core.GetId( gp )                      << endl;
		buffer << L"En: " << setw( 5 ) << core.GetEnergy( gp )                  << endl;
		buffer << L"Age:" << setw( 5 ) << core.GetAge( gp )                     << endl;
		buffer << L"Or: " << setw( 5 ) << GetOriginName( core.GetOrigin( gp ) ) << endl;

		PlannedActivity plan = core.GetPlan( );
		if ( plan.IsValid( ) )
		{
			if ( (gp == plan.GetActor( )) || (gp == plan.GetPartner( )) )
			{
				buffer << (( gp == plan.GetActor( ) ) ? L"** ACTOR **" : L"** PARTNER **") << endl;
				buffer << L"** "  << GetActionTypeName( plan.GetActionType( ) ) << L" **"  << endl;
				buffer << L"BaseCons: " << plan.GetBaseConsumption( )                      << endl;
			}
		}
	}

private:
};
