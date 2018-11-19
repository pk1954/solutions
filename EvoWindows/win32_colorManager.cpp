// win32_colorManager.cpp
//

#include "stdafx.h"
#include "Windows.h"
#include "Commdlg.h"
#include "debug.h"
#include "config.h"
#include "win32_util.h"
#include "win32_colorManager.h"

ColorManager::ColorManager( ) :
	m_bDimmIndividuals( TRUE ),
	m_colorSelection( RGB(   0, 217, 255) ),
	m_colorHighlight( RGB( 255, 217,   0) )
{
	UINT uiClutSize = static_cast<UINT>(Config::GetConfigValue( Config::tId::stdCapacity ));
	for ( auto & strategy : m_aClutStrat )
		strategy.Allocate( uiClutSize );

	setStrategyColor( tStrategyId::defect,    RGB( 20, 150, 187) );
	setStrategyColor( tStrategyId::cooperate, RGB(130, 147,  86) );
	setStrategyColor( tStrategyId::tit4tat,   RGB(192,  47,  29) );

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

void ColorManager::ColorDialog
( 
	HWND         const hwndOwner, 
	tColorObject const object, 
	tStrategyId  const strat 
)
{
	static COLORREF acrCustClr[16]; // array of custom colors 
	CHOOSECOLOR cc;
		
	ZeroMemory( &cc, sizeof( cc ) );
	cc.lStructSize  = sizeof( cc ) ;
	cc.hwndOwner    = hwndOwner;
	cc.lpCustColors = (LPDWORD)acrCustClr;
	cc.Flags        = CC_RGBINIT;
	cc.rgbResult    = GetColor( object, strat );

	if ( ChooseColor( & cc ) )
	{
		SetColor( cc.rgbResult, object, strat );
	}
}

void ColorManager::SetColor
(
	COLORREF     const color,
	tColorObject const object,
	tStrategyId  const strat
)
{
	switch ( object )
	{
	case tColorObject::individual:
		setStrategyColor( strat, color );
		break;
	case tColorObject::selection:
		m_colorSelection = color;
		break;
	case tColorObject::highlight:
		m_colorHighlight = color;
		break;
	default:
		assert( false );
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

COLORREF ColorManager::GetColor
( 
	tColorObject const object, 
	tStrategyId  const strat, 
	UINT         const uiClutIndex
)
{
	switch (object)
	{
	case tColorObject::individual:
	{
		if ( uiClutIndex == -1 )
			return getStrategyColor( strat );
		else
		{
			CLUT const & clut = m_aClutStrat.at( static_cast<int>( strat ) );
			assert( uiClutIndex < clut.GetSize() ); 
			COLORREF dwColor = clut.GetColor( uiClutIndex );
			return dwColor;
		}
	}

	case tColorObject::selection:
		return m_colorSelection;
		break;

	case tColorObject::highlight:
		return m_colorHighlight;

	default:
		assert( false );
		return 0;
	}
}
