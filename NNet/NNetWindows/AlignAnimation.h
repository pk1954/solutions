// AlignAnimation.h
//
// NNetWindows

#pragma once

#include <vector>
#include "ConnectionNeuron.h"
#include "MicroMeterPointVector.h"
#include "win32_animation.h"

using std::vector;
using std::unique_ptr;

class NNetModelWriterInterface;
class NNetModelCommands;

class AlignAnimation
{
public:
	void Initialize
	( 
		HWND const,
		NNetModelWriterInterface &,
		NNetModelCommands &
	);

	static DWORD const ALIGN_DIRECTION  { 0x01 };
	static DWORD const ALIGN_SHAPES     { 0x02 };
	static DWORD const PACK_SHAPES      { 0x04 };
	static DWORD const CREATE_CONNECTOR { 0x08 };

	bool            const AlignSelection( AnimationScript const & );
	void                  AnimationStep();
	bool            const NextStep();
	wchar_t const * const DoNextStep();

private:

	struct ALIGN_PNT
	{
		ConnNeuron * pConnectionNeuron;
		MicroMeter   umDist;
	};

	using ALIGN_VECTOR = vector<ALIGN_PNT>;

	using ConnAnimation = Animation<MicroMeterPointVector>;

	NNetModelWriterInterface * m_pNMWI          { nullptr };
	NNetModelCommands        * m_pModelCommands { nullptr };
	MicroMeterLine             m_line           { MicroMeterLine::NULL_VAL() };
	MicroMeterPoint            m_orthoVector    { MicroMeterPoint::NULL_VAL() };
	DWORD                      m_animationPhase { AlignAnimation::ALIGN_DIRECTION };

	AnimationScript    const * m_pScript { nullptr };
	int                        m_iScriptStep { -1 };

	unique_ptr<ConnAnimation> m_upConnAnimation;
	ShapePtrList<ConnNeuron>  m_shapesAnimated;

	bool const prepareData();
	void       calcDistances  (ALIGN_VECTOR &);
	void       sortDistances  (ALIGN_VECTOR &);
	bool const calcMaxDistLine(ALIGN_VECTOR const &);
	void       scriptStep     ();

};
