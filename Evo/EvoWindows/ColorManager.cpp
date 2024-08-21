// ColorManager.cpp
//
// EvoWindows

module;

#include "Windows.h"
#include "Commdlg.h"

module ColorManager;

import Config;

void ColorManager::Initialize()
{
	m_bDimmIndividuals = TRUE;
	m_colorSelection = RGB(  0, 217, 255);
	m_colorHighlight = RGB(255, 217,   0);

	CLUT_INDEX clutSize { Config::GetConfigValue(Config::tId::stdCapacity) };
	for (auto & strategy : m_aClutStrat)
		strategy.Allocate(clutSize);

	setStrategyColor(Strategy::Id::defect,    RGB(20, 150, 187));
	setStrategyColor(Strategy::Id::cooperate, RGB(130, 147,  86));
	setStrategyColor(Strategy::Id::tit4tat,   RGB(192,  47,  29));

    setupClut(Config::GetConfigValueBoolOp(Config::tId::dimmMode));
}

void ColorManager::setupClut(tBoolOp const bOp)
{
    ApplyOp(m_bDimmIndividuals, bOp);

    CLUT_INDEX const clutBase = m_bDimmIndividuals // color of individuals ...
                                ? CLUT_INDEX(30)   // ... varies from 30% - 100%, depending on energy 
                                : CLUT_INDEX(100); // ... is always at 100%

    for (auto &strategy : m_aClutStrat)
        strategy.SetClutBase(clutBase);
}

void ColorManager::ToggleClutMode() 
{ 
	setupClut(tBoolOp::opToggle); 
}

void ColorManager::ColorDialog
(
	HWND         const hwndOwner, 
	tColorObject const object, 
	Strategy::Id const strat 
)
{
	static COLORREF acrCustClr[16]; // array of custom colors 
	CHOOSECOLOR cc;
		
	ZeroMemory(&cc, sizeof(cc));
	cc.lStructSize  = sizeof(cc) ;
	cc.hwndOwner    = hwndOwner;
	cc.lpCustColors = (LPDWORD)acrCustClr;
	cc.Flags        = CC_RGBINIT;
	cc.rgbResult    = GetColor(object, strat);

	if (ChooseColor(& cc))
	{
		SetColor(cc.rgbResult, object, strat);
	}
}

void ColorManager::SetColor
(
	COLORREF     const color,
	tColorObject const object,
	Strategy::Id const strat
)
{
	switch (object)
	{
	case tColorObject::individual:
		setStrategyColor(strat, color);
		break;
	case tColorObject::selection:
		m_colorSelection = color;
		break;
	case tColorObject::highlight:
		m_colorHighlight = color;
		break;
	default:
		Assert(false);
	}
}

COLORREF ColorManager::GetColor
(
	tColorObject const object, 
	Strategy::Id const strat, 
	CLUT_INDEX   const clutIndex
)
{
	switch (object)
	{
	case tColorObject::individual:
	{
		return (clutIndex == STRATEGY_COLOR())
			   ? getStrategyColor(strat)
			   : getClut(strat).GetColor(clutIndex);
	}

	case tColorObject::selection:
		return m_colorSelection;
		break;

	case tColorObject::highlight:
		return m_colorHighlight;

	default:
		Assert(false);
		return 0;
	}
}
