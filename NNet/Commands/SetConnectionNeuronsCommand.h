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
		MicroMeterPointVector const & umPntVector,
		ShapePtrList<ConnNeuron>    & shapes2Animate
	)
	  : m_umPntVector(umPntVector),
		m_shapes2Animate(shapes2Animate)
	{}

	virtual void Do( NNetModelWriterInterface & nmwi ) 
	{ 
		unsigned int ui = 0;
		m_shapes2Animate.Apply2All
		(
			[&](ConnNeuron & n)
			{
				MicroMeterPosDir const posDir { n.GetPosDir() };
				n.SetPosDir( m_umPntVector.GetPosDir(ui) );
				m_umPntVector.SetPosDir( ui, posDir );
				++ui;
			}
		);
	}

private:
	MicroMeterPointVector    m_umPntVector;
	ShapePtrList<ConnNeuron> m_shapes2Animate;
};
