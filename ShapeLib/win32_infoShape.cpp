// win32_infoShape.cpp
//

#include "stdafx.h"
#include "config.h"
#include "win32_infoShape.h"

using std::wostringstream;
using std::setw;
using std::endl;

void InfoShape::FillBuffer( EvolutionCore const * const pCore, GridPoint const gp )
{
	wostringstream & buffer = m_textDisplay.Buffer();

	buffer << L"En: " << setw( 5 ) << pCore->GetEnergy( gp )                  << endl;
	buffer << L"Age:" << setw( 5 ) << pCore->GetAge( gp )                     << endl;
	buffer << L"Or: " << setw( 5 ) << GetOriginName( pCore->GetOrigin( gp ) ) << endl;

	PlannedActivity plan = pCore->GetPlan( );
	if ( plan.IsValid( ) )
	{
		if ( (gp == plan.GetActor( )) || (gp == plan.GetPartner( )) )
		{
			buffer << (( gp == plan.GetActor( ) ) ? L"** ACTOR **" : L"** PARTNER **") << endl;
			buffer << L"** "  <<  Action::GetName( plan.GetActionType( ) ) << L" **"   << endl;
			buffer << L"BaseCons: " << plan.GetBaseConsumption( )                      << endl;
		}
	}
}
