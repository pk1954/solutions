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
		unique_ptr<ShapeIdList>   upShapeIds
	)
	  : m_umPntVector(umPntVector),
		m_upShapeIds(move(upShapeIds))
	{}

	virtual void Do( NNetModelWriterInterface & nmwi ) 
	{ 
		unsigned int ui = 0;
		m_upShapeIds->Apply2All
		(
			[&](ShapeId const & id)
			{
				ConnNeuron     * const pConnNeuron { nmwi.GetShapePtr<ConnNeuron *>(id) };
				MicroMeterPosDir const posDir      { pConnNeuron->GetRawPosDir() };
				pConnNeuron->SetPosDir( m_umPntVector.GetPosDir(ui) );
				m_umPntVector.SetPosDir( ui, posDir );
				++ui;
			}
		);
	}

private:
	MicroMeterPointVector   m_umPntVector;
	unique_ptr<ShapeIdList> m_upShapeIds;
};
