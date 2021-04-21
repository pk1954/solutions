// AlignPositionAnimation.h
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

class AlignPositionAnimation
{
public:
	void Initialize
	( 
		HWND const,
		NNetModelWriterInterface &,
		NNetModelCommands &,
		Sound *
	);

	void StartAnimation();
	void AnimationStep();

private:

	using ConnAnimation = Animation<MicroMeterPointVector>;

	NNetModelWriterInterface * m_pNMWI          { nullptr };
	NNetModelCommands        * m_pModelCommands { nullptr };
	MicroMeterLine             m_line           { MicroMeterLine::NULL_VAL() };
	Sound                   *  m_pSound         { nullptr };

	unique_ptr<ConnAnimation> m_upConnAnimation;
	ShapePtrList<ConnNeuron>  m_shapesAnimated;

	bool         const prepareData();
	void               doAnimation();
	unsigned int const calcNrOfSteps(MicroMeterPointVector const &, MicroMeterPointVector const &) const;
};
