// SetConnectionNeuronsCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "MicroMeterPointVector.h"
#include "NNetModelWriterInterface.h"
#include "ShapePtrList.h"
#include "Command.h"
#include "ConnectionNeuron.h"

class SetConnectionNeuronsCommand : public Command
{
public:
	SetConnectionNeuronsCommand
	(
		MicroMeterPointVector    const & umPntVector,
		ShapePtrList<ConnectionNeuron> & shapes2Animate
	)
	  : m_umPntVector(umPntVector),
		m_shapes2Animate(shapes2Animate)
	{}

	virtual void Do( NNetModelWriterInterface & nmwi ) 
	{ 
		unsigned int ui = 0;
		m_shapes2Animate.Apply2All
		(
			[&](ConnectionNeuron & connectionNeuron)
			{
				MicroMeterPosDir const posDir { connectionNeuron.GetPosDir() };
				connectionNeuron.SetPosDir( m_umPntVector.GetPosDir(ui) );
				m_umPntVector.SetPosDir( ui, posDir );
				++ui;
			}
		);
	}

private:
	MicroMeterPointVector          m_umPntVector;
	ShapePtrList<ConnectionNeuron> m_shapes2Animate;
};
