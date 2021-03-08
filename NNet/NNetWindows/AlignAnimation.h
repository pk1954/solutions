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

	bool AlignSelection();
	void AnimationStep();

private:

	struct ALIGN_PNT
	{
		ConnectionNeuron * pConnectionNeuron;
		MicroMeter         umDist;
	};

	using ALIGN_VECTOR = vector<ALIGN_PNT>;

	using ConnectorAnimation = Animation<MicroMeterPointVector>;

	NNetModelWriterInterface     * m_pNMWI         { nullptr };
	NNetModelCommands            * m_pNNetCommands { nullptr };
	unique_ptr<ConnectorAnimation> m_upConnectorAnimation;
	ShapePtrList<ConnectionNeuron> m_shapesAnimated;

	static MicroMeterLine const calcMaxDistLine(ALIGN_VECTOR const &);
	static MicroMeterLine const prepareData(ShapePtrList<ConnectionNeuron> &, UPShapeList &);
};
