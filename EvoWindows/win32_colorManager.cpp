// win32_colorManager.cpp
//

#include "stdafx.h"
#include "debug.h"
#include "config.h"
#include "win32_util.h"
#include "win32_colorManager.h"

ColorManager::ColorManager( )
{
	m_bDimmIndividuals = TRUE;

	UINT uiClutSize = static_cast<UINT>(Config::GetConfigValue( Config::tId::stdCapacity ));
	for ( auto & strategy : m_aClutStrat )
		strategy.Allocate( uiClutSize );

	setStrategyColor( tStrategyId::defect,    RGB(151, 171, 255) );
	setStrategyColor( tStrategyId::cooperate, RGB(127, 255,   0) );
	setStrategyColor( tStrategyId::tit4tat,   RGB(255,  50,  50) );

    setupClut( Config::GetConfigValueBoolOp( Config::tId::dimmMode ) );
}

void ColorManager::setupClut( tBoolOp const bOp )
{
    ApplyOp( m_bDimmIndividuals, bOp );

    UINT const uiClutBase = m_bDimmIndividuals // color of individuals ...
                          ? 30       // ... varies from 30% - 100%, depending on energy 
                          : 100;     // ... is always at 100%

    for ( auto &strategy : m_aClutStrat )
        strategy.SetClutBase( uiClutBase );
}

void ColorManager::ToggleClutMode( ) 
{ 
	setupClut( tBoolOp::opToggle ); 
}

void ColorManager::ColorDialog( HWND const hwndOwner, tStrategyId const strat )
{
	static COLORREF acrCustClr[16]; // array of custom colors 
	CHOOSECOLOR cc;
		
	ZeroMemory( &cc, sizeof( cc ) );
	cc.lStructSize  = sizeof( cc ) ;
	cc.hwndOwner    = hwndOwner;
	cc.lpCustColors = (LPDWORD)acrCustClr;
	cc.rgbResult    = getStrategyColor( strat );
	cc.Flags        = CC_RGBINIT;

	if ( ChooseColor( & cc ) )
	{
	    setStrategyColor( strat, cc.rgbResult );
	}
}

COLORREF ColorManager::getStrategyColor( tStrategyId const strat )
{
	int iIndex = static_cast<int>(strat);
	ASSERT_LIMITS( iIndex, 0, NR_STRATEGIES - 1 );
	return m_aClutStrat[ iIndex ].GetColorHi( );
}

void ColorManager::setStrategyColor( tStrategyId const strat, COLORREF const col )
{
    m_aClutStrat.at( static_cast<int>(strat) ).SetColorHi( col );
}

DWORD ColorManager::GetColorFromClut( tStrategyId const strat, UINT const uiClutIndex )
{
    CLUT const & clut = m_aClutStrat.at( static_cast<int>( strat ) );
    assert( uiClutIndex < clut.GetSize() ); 
	DWORD dwColor = clut.Get( uiClutIndex );
	return dwColor;
}
