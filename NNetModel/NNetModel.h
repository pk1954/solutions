// NNetModel.h 
//
// NNetModel

#pragma once

#include <vector>
#include <functional>
#include "util.h"
#include "MoreTypes.h"
#include "Segment.h"
#include "InputNeuron.h"
#include "Neuron.h"
#include "Pipeline.h"
#include "ModelInterface.h"

class ObserverInterface;
class EventInterface;
class Knot;

class NNetModel : public ModelInterface
{
public:
	NNetModel();

	virtual ~NNetModel( );

	// readOnly functions

	BYTES const GetCoreSize() const 
	{ 
		return BYTES(sizeof(NNetModel)); 
	};

	microseconds GetSimulationTime( ) const 
	{
		return m_timeStamp;
	}

	Shape * GetShape( ShapeId const id )
	{
		return ( id == NO_SHAPE ) ? nullptr : m_Shapes[ id.GetValue() - 1 ];
	}

	Shape const * GetConstShape( ShapeId const id ) const
	{
		return ( id == NO_SHAPE ) ? nullptr : m_Shapes[ id.GetValue() - 1 ];
	}

	Shape const * GetShapeUnderPoint( MicroMeterPoint const ) const;

	Pipeline * GetPipeline( ShapeId const id ) 
	{
		Shape * pShape = GetShape( id );
		assert( pShape );
		assert( pShape->GetShapeType() == tShapeType::pipeline );
		return static_cast<Pipeline *>( pShape );
	}

	Pipeline const * GetConstPipeline( ShapeId const id ) const
	{
		Shape const * pShape { GetConstShape( id ) };
		assert( pShape );
		assert( pShape->GetShapeType() == tShapeType::pipeline );
		return static_cast<Pipeline const *>( pShape );
	}

	BaseKnot * GetBaseKnot( ShapeId const id ) 
	{
		Shape * pShape = GetShape( id );
		assert( pShape );
		assert( IsBaseKnotType( pShape->GetShapeType() ) );
		return static_cast<BaseKnot *>( pShape );
	}

	BaseKnot const * GetConstBaseKnot( ShapeId const id ) const
	{
		Shape const * pShape = GetConstShape( id );
		assert( pShape );
		assert( IsBaseKnotType( pShape->GetShapeType() ) );
		return static_cast<BaseKnot const *>( pShape );
	}

	void AddIncomming( ShapeId const, ShapeId const );
	void AddOutgoing ( ShapeId const, ShapeId const );
		
	// manipulating functions

	ShapeId const AddInputNeuron( MicroMeterPoint const & );
	ShapeId const AddNeuron     ( MicroMeterPoint const & );
	ShapeId const AddKnot       ( MicroMeterPoint const & );
	ShapeId const AddPipeline   ( meterPerSec     const );

	void HighlightShape( ShapeId const );
	void Apply2AllShapes( std::function<void(Shape * const)> const & ) const;

	virtual void CopyModelData( ModelInterface const * const );
	virtual void Compute( );
	virtual void ResetAll( );

private:
	ShapeId m_idNeuron1;  
	ShapeId m_idKnot1;    
	ShapeId m_idKnot2;    
	ShapeId m_idKnot3;    
	ShapeId m_idKnot4;    
	ShapeId m_idPipeline1;
	ShapeId m_idPipeline2;
	ShapeId m_idPipeline3;
	ShapeId m_idPipeline4;
			  
	microseconds    m_timeStamp;
	vector<Shape *> m_Shapes;
	ShapeId         m_shapeHighlighted;

	ShapeId const addShape( Shape * );
};
