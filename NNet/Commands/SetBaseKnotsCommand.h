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
		MicroMeterPointVector  & umPntVectorRun,
		ShapePtrList<BaseKnot> & shapes2Animate
	)
	  : m_umPntVectorRun(umPntVectorRun),
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
				baseKnot.SetPosition( m_umPntVectorRun.GetPos(ui) );
				m_umPntVectorRun.SetPosition( ui, umPnt );
				++ui;
			}
		);
	}

private:
	MicroMeterPointVector  m_umPntVectorRun;
	ShapePtrList<BaseKnot> m_shapes2Animate;
};
