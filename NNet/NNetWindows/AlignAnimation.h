// AlignAnimation.h
//
// NNetWindows

#pragma once

#include "BaseKnot.h"
#include "MicroMeterPointVector.h"
#include "win32_animation.h"

using std::unique_ptr;

class NNetModelReaderInterface;
class NNetModelCommands;

class AlignAnimation
{
public:
	AlignAnimation
	( 
		NNetModelReaderInterface const &,
		NNetModelCommands &,
		HWND const,
		int  const
	);

	bool AlignSelection();
	void AnimationStep();

private:

	NNetModelReaderInterface             const * m_pNMRI         { nullptr };
	NNetModelCommands                          * m_pNNetCommands { nullptr };
	unique_ptr<Animation<MicroMeterPointVector>> m_upConnectorAnimation;
	ShapePtrList<BaseKnot>                       m_shapesAnimated;
};
