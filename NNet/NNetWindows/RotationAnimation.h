// RotationAnimation.h
//
// NNetWindows

#pragma once

#include <vector>
#include "Connector.h"
#include "MicroMeterPointVector.h"
#include "win32_animation.h"

using std::vector;
using std::unique_ptr;

class NNetModelWriterInterface;
class NNetModelCommands;

class RotationAnimation
{
public:
	void Initialize
	( 
		HWND const,
		NNetModelWriterInterface &,
		NNetModelCommands &
	);

	bool const RotateSelection( Connector const & );
	void       AnimationStep();

private:

	using RotAnimation = Animation<MicroMeterPointVector>;

	NNetModelWriterInterface * m_pNMWI          { nullptr };
	NNetModelCommands        * m_pModelCommands { nullptr };
	MicroMeterLine             m_line           { MicroMeterLine::NULL_VAL() };
	MicroMeterPoint            m_orthoVector    { MicroMeterPoint::NULL_VAL() };

	unique_ptr<RotAnimation> m_upRotAnimation;
	Connector              * m_connectorAnimated;

	bool const prepareData();
};
