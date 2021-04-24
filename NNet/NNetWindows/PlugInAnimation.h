// PluginAnimation.h
//
// NNetWindows

#pragma once

#include "MicroMeterPosDir.h"
#include "win32_animation.h"

class NNetModelWriterInterface;
class NNetModelCommands;

class PlugInAnimation
{
public:
    void Initialize
	( 
		ShapeId const,
		HWND    const,
		NNetModelWriterInterface &,
		NNetModelCommands &
	);

	static DWORD const MOVE_TO_START_POS { 0x01 };
	static DWORD const PLUG_IN           { 0x02 };

	bool            const StartAnimation( AnimationScript const & );
	void                  AnimationStep();
	bool            const NextScriptStep();
	wchar_t const * const DoNextStep();

private:

	AnimationScript    const * m_pScript { nullptr };
	int                        m_iScriptStep { -1 };

	ShapeId                    m_idShape           { NO_SHAPE };
	NNetModelWriterInterface * m_pNMWI             { nullptr };
	NNetModelCommands        * m_pModelCommands    { nullptr };
	MicroMeterPosDir           m_umPosDirAnimation { };

	unique_ptr<Animation<MicroMeterPointVector>>  m_upConnAnimation;

	void scriptStep();
};