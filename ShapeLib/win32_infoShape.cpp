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
	ENERGY_UNITS    energy   { 12345 };
	EVO_GENERATION  evoGen   { 0 };
	wchar_t const * origin   { L"" };
	wchar_t const * strategy { L"" };

	if (gp.IsNotZero())
	{
		energy   = pCore->GetEnergy      ( gp );
		evoGen   = pCore->GetAge         ( gp );
		origin   = pCore->GetOriginName  ( gp );
		strategy = pCore->GetStrategyName( gp );
	}

	wostringstream & buffer = m_textDisplay.Buffer();
	buffer << L"En: " << setw( 5 ) << energy << endl;
	buffer << L"Age:" << setw( 5 ) << evoGen << endl;
	buffer << L"Or: " << setw( 5 ) << origin << endl;
	buffer << L"Str:" << setw( 5 ) << strategy;

	PlannedActivity plan = pCore->GetPlan( gp );
//	if ( plan.IsValid( ) )
	{
		if ( (gp == plan.GetActor( )) || (gp == plan.GetPartner( )) )
		{
			buffer << endl;
			buffer << (( gp == plan.GetActor( ) ) ? L"** ACTOR **" : L"** PARTNER **") << endl;
			buffer << L"** "  <<  Action::GetName( plan.GetActionType( ) ) << L" **"   << endl;
			buffer << L"BaseCons: " << plan.GetBaseConsumption( );
		}
	}
}
