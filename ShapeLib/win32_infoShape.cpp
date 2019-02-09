// win32_infoShape.cpp
//

#include "stdafx.h"
#include "config.h"
#include "win32_infoShape.h"

using std::wostringstream;
using std::setw;
using std::endl;

void InfoShape::FillBuffer( GridPoint const gp )
{
	wostringstream & buffer = m_textDisplay.Buffer();
	EvolutionCore  & core   = m_textDisplay.Core();

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
