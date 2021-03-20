// AlignAnimation.h
//
// NNetWindows

#pragma once

#include "ConnectionNeuron.h"
#include "MicroMeterPointVector.h"
#include "win32_animation.h"

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
	static DWORD const CREATE_CONNECTOR { 0x0f };  // includes all other options

	bool const AlignSelection( DWORD const );
	bool const AnimationStep( bool const );

private:

	struct ALIGN_PNT
	{
		ConnectionNeuron * pConnectionNeuron;
		MicroMeter         umDist;
	};

	using ALIGN_VECTOR = vector<ALIGN_PNT>;

	using ConnectorAnimation = Animation<MicroMeterPointVector>;

	NNetModelWriterInterface * m_pNMWI           { nullptr };
	NNetModelCommands        * m_pNNetCommands   { nullptr };
	MicroMeterLine             m_line            { MicroMeterLine::NULL_VAL() };
	MicroMeterPoint            m_orthoVector     { MicroMeterPoint::NULL_VAL() };
	DWORD                      m_animationPhase  { AlignAnimation::ALIGN_DIRECTION };

	bool m_bAlignDirection  { false };
	bool m_bAlignShapes     { false };
	bool m_bPackShapes      { false };
	bool m_bCreateConnector { false };

	unique_ptr<ConnectorAnimation> m_upConnectorAnimation;
	ShapePtrList<ConnectionNeuron> m_shapesAnimated;

	bool const prepareData();
	void       calcDistances  (ALIGN_VECTOR &);
	void       sortDistances  (ALIGN_VECTOR &);
	bool const calcMaxDistLine(ALIGN_VECTOR const &);
	void       calcOrthoVector(ShapePtrList<ConnectionNeuron> const &);
};
