// SetBaseKnotsCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "MicroMeterPointVector.h"
#include "NNetModelWriterInterface.h"
#include "ShapePtrList.h"
#include "Command.h"
#include "BaseKnot.h"

class SetBaseKnotsCommand : public Command
{
public:
	SetBaseKnotsCommand
	( 
		MicroMeterPointVector  const & umPntVector,
		ShapePtrList<BaseKnot>       & shapes2Animate
	)
	  : m_umPntVector(umPntVector),
		m_shapes2Animate(shapes2Animate)
	{}

	virtual void Do( NNetModelWriterInterface & nmwi ) 
	{ 
		unsigned int ui = 0;
		m_shapes2Animate.Apply2All
		(
			[&](BaseKnot & baseKnot)
			{
				MicroMeterPoint umPnt { baseKnot.GetPosition() };
				baseKnot.SetPosition( m_umPntVector.GetPos(ui) );
				m_umPntVector.SetPosition( ui, umPnt );
				++ui;
			}
		);
	}

private:
	MicroMeterPointVector  m_umPntVector;
	ShapePtrList<BaseKnot> m_shapes2Animate;
};
