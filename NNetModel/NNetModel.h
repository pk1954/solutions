// NNetModel.h 
//
// NNetModel

#pragma once

#include <vector>
#include <functional>
#include "util.h"
#include "MoreTypes.h"
#include "PixelCoordsFp.h"
#include "Segment.h"
#include "InputNeuron.h"
#include "OutputNeuron.h"
#include "Neuron.h"
#include "Knot.h"
#include "Pipeline.h"
#include "ModelInterface.h"

class ObserverInterface;
class EventInterface;

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

	void ResetSimulationTime( )
	{
		m_timeStamp = microseconds( 0 );
	}

	Shape * GetShape( ShapeId const id )
	{
		return ( id == NO_SHAPE ) ? nullptr : m_Shapes[ id.GetValue() - 1 ];
	}

	Shape const * GetConstShape( ShapeId const id ) const
	{
		return ( id == NO_SHAPE ) ? nullptr : m_Shapes[ id.GetValue() - 1 ];
	}

	Shape const * FindShapeUnderPoint( MicroMeterPoint const, std::function<bool(Shape const &)> const & ) const;
	Shape const * FindShapeUnderPoint( MicroMeterPoint const ) const;
	
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

	Knot * GetKnot( ShapeId const id ) 
	{
		Shape * pShape = GetShape( id );
		assert( pShape );
		assert( pShape->GetShapeType() == tShapeType::knot );
		return static_cast<Knot *>( pShape );
	}

	Knot const * GetConstKnot( ShapeId const id ) const
	{
		Shape const * pShape = GetConstShape( id );
		assert( pShape );
		assert( pShape->GetShapeType() == tShapeType::knot );
		return static_cast<Knot const *>( pShape );
	}

	void AddIncomming( ShapeId const, ShapeId const );
	void AddOutgoing ( ShapeId const, ShapeId const );
		
	// manipulating functions

	ShapeId const AddInputNeuron ( MicroMeterPoint const & );
	ShapeId const AddOutputNeuron( MicroMeterPoint const & );
	ShapeId const AddNeuron      ( MicroMeterPoint const & );
	ShapeId const AddKnot        ( MicroMeterPoint const & );
	ShapeId const AddPipeline    ( meterPerSec     const );

	void CreateNewBranch( ShapeId const );
	void CreateNewNeuron( MicroMeterPoint const & );

	void HighlightShape     ( ShapeId const );
	void SuperHighlightShape( ShapeId const );

	void Apply2AllShapes   ( std::function<void(Shape &)> const & ) const;
	void Apply2AllNeurons  ( std::function<void(Shape &)> const & ) const;
	void Apply2AllPipelines( std::function<void(Shape &)> const & ) const;

	ShapeId GetHighlightedShapeId( ) const
	{
		return m_shapeHighlighted;
	}

	virtual void CopyModelData( ModelInterface const * const );
	virtual void Compute( );
	virtual void ResetAll( );

private:
	ShapeId m_idInputNeuron1;
	ShapeId m_idKnot1;
	ShapeId m_idNeuron1;
	ShapeId m_idOutputNeuron1;
	ShapeId m_idOutputNeuron2;
	ShapeId m_idPipeline1;
	ShapeId m_idPipeline2;
	ShapeId m_idPipeline3;
	ShapeId m_idPipeline4;
			  
	microseconds    m_timeStamp;
	vector<Shape *> m_Shapes;
	ShapeId         m_shapeHighlighted;
	ShapeId         m_shapeSuperHighlighted;

	ShapeId const addShape( Shape * );
};
