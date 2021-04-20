// PlugInAnimation.cpp
//
// NNetWindows

#include "stdafx.h"
#include "Resource.h"
#include "NNetModelWriterInterface.h"
#include "NNetModelCommands.h"
#include "PlugInAnimation.h"

void PlugInAnimation::Initialize
( 
	ShapeId            const   id,
	HWND               const   hwnd,
	NNetModelWriterInterface & nmwi,
	NNetModelCommands        & commands
)
{
	m_pNMWI           = & nmwi;
	m_pModelCommands  = & commands;
	m_upConnAnimation = make_unique<ConnAnimation>(IDX_PLUG_IN_ANIMATION, hwnd); 
}


void PlugInAnimation::scriptStep( )
{
	// TODO
}

bool const PlugInAnimation::AlignSelection( AnimationScript const & script )
{
	m_pScript = & script;
	m_iScriptStep = 0;

	scriptStep();
	return true;
}

void PlugInAnimation::AnimationStep()
{
//	m_pModelCommands->SetShape(m_idShape, );
}

bool const PlugInAnimation::NextScriptStep()
{
	return ++m_iScriptStep < m_pScript->size();
}

wchar_t const * const PlugInAnimation::DoNextStep()
{
	if (m_pScript->at(m_iScriptStep) & PLUG_IN)
	{
//		m_pModelCommands->CreateConnector( m_shapesAnimated );
		return L"SNAP_IN_SOUND";
	}
	else
	{
		scriptStep();
		return nullptr;
	}
}
