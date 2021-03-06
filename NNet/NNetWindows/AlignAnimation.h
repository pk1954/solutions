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
		int  const,
		HWND const,
		NNetModelReaderInterface const &,
		NNetModelCommands &
	);

	bool AlignSelection();
	void AnimationStep();

private:
	using ConnectorAnimation = Animation<MicroMeterPointVector>;

	NNetModelReaderInterface const * m_pNMRI         { nullptr };
	NNetModelCommands              * m_pNNetCommands { nullptr };
	unique_ptr<ConnectorAnimation>   m_upConnectorAnimation;
	ShapePtrList<BaseKnot>           m_shapesAnimated;

	ShapeType const determineShapeType();
};
