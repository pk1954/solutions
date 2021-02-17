// SetBaseKnotsCommand.h
//
// Commands

#pragma once

#include "MoreTypes.h"
#include "MicroMeterPointVector.h"
#include "NNetModelWriterInterface.h"
#include "Command.h"
#include "BaseKnot.h"

class SetBaseKnotsCommand : public Command
{
public:
	SetBaseKnotsCommand
	( 
		NNetModelWriterInterface & nmwi, 
		MicroMeterPointVector    & list 
	)
		: m_list(list)
	{}

	virtual void Do( NNetModelWriterInterface & nmwi ) 
	{ 
		unsigned int ui = 0;
		nmwi.GetUPShapes().Apply2AllSelected<BaseKnot>
		( 
			[&]( BaseKnot & knot )
			{ 
				MicroMeterPoint umPnt { knot.GetPosition() };
				knot.SetPosition( m_list.GetPos(ui) );
				m_list.SetPos( ui, umPnt );
				++ui;
			} 
		);
	}

private:
	MicroMeterPointVector m_list;
};
