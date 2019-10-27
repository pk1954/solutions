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

	Shape const * FindShapeUnderPoint( MicroMeterPoint const, std::function<bool(Shape const &)> const & ) const;
	Shape const * FindShapeUnderPoint( MicroMeterPoint const ) const;
	
	Shape          * GetShape        ( ShapeId const );
	Shape    const * GetConstShape   ( ShapeId const ) const;
	Pipeline       * GetPipeline     ( ShapeId const ); 
	Pipeline const * GetConstPipeline( ShapeId const ) const;
	BaseKnot       * GetBaseKnot     ( ShapeId const ); 
	BaseKnot const * GetConstBaseKnot( ShapeId const ) const;
	Knot           * GetKnot         ( ShapeId const ); 
	Knot     const * GetConstKnot    ( ShapeId const ) const;

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
	void CreateNewInputNeuron( MicroMeterPoint const & );
	void CreateNewOutputNeuron( MicroMeterPoint const & );
	void Connect( );

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
	void          deleteShape( ShapeId const );
	void          checkConsistency( Shape * ) const;
};
