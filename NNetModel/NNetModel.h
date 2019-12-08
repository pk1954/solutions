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
#include "OutputNeuron.h"
#include "Neuron.h"
#include "Knot.h"
#include "Pipeline.h"
#include "ModelInterface.h"

class ObserverInterface;
class EventInterface;

enum class tParameter
{
	pulseRate,
	pulseSpeed,
	pulseWidth,
	dampingFactor,
	threshold,
	peakVoltage,
	refractoryPeriod,
	tParameterLast = refractoryPeriod
};

class NNetModel : public ModelInterface
{
public:
	NNetModel();

	virtual ~NNetModel( );

	class ModelInconsistencyException : public std::exception {};
	static ModelInconsistencyException const ModelInconsistency;

	// readOnly functions

	void CheckConsistency( Shape const * ) const;

	ShapeId GetId( Shape const * pShape ) const
	{
		return pShape ? pShape->GetId( ) : NO_SHAPE;
	}

	bool IsValidShapeId( ShapeId id ) const { return id.GetValue() < m_Shapes.size(); }

	Shape * GetShape( ShapeId const id )
	{
		if ( ! IsDefined( id ) )
			return nullptr;
		if ( ! IsValidShapeId( id ) )
			throw ModelInconsistency;
		return m_Shapes[ id.GetValue() ];
	}

	Shape const * GetConstShape( ShapeId const id ) const
	{
		if ( ! IsDefined( id ) )
			return nullptr;
		if ( ! IsValidShapeId( id ) )
			throw ModelInconsistency;
		return m_Shapes[ id.GetValue() ];
	}

	template <typename T>
	T * GetTypedShape( ShapeId const id ) 
	{
		Shape * pShape = GetShape( id );
		return ( pShape && T::TypeFits( pShape->GetShapeType() ) ) 
			? static_cast<T *>( pShape )
			: nullptr;
	}

	template <typename T>
	T const * GetConstTypedShape( ShapeId const id ) const
	{
		Shape const * pShape = GetConstShape( id );
		return ( pShape && T::TypeFits( pShape->GetShapeType() ) ) 
			? static_cast<T const *>( pShape )
			: nullptr;
	}

	BYTES     const GetCoreSize()                 const { return BYTES(sizeof(NNetModel)); };
	MicroSecs const GetSimulationTime( )          const { return m_timeStamp; }
	ShapeId   const GetHighlightedShapeId( )      const { return m_shapeHighlighted; }
	ShapeId   const GetSuperHighlightedShapeId( ) const { return m_shapeSuperHighlighted; }

	tShapeType const GetHighlightedShapeType( ) const 
	{ 
		Shape const * const pHighlighted{ GetConstShape( m_shapeHighlighted ) };
		return ( pHighlighted ) ? pHighlighted->GetShapeType( ) : tShapeType::undefined;
	}

	tShapeType const GetSuperHighlightedShapeType( ) const 
	{ 
		Shape const * const pSuperHighlighted{ GetConstShape( m_shapeSuperHighlighted ) };
		return ( pSuperHighlighted ) ? pSuperHighlighted->GetShapeType( ) : tShapeType::undefined;
	}

	MicroMeterPoint const GetHighlightedShapePos( ) const 
	{ 
		return IsDefined( m_shapeHighlighted ) 
			? GetConstTypedShape<BaseKnot>( m_shapeHighlighted )->GetPosition()
			: NP_NULL;
	}

	COLORREF const GetFrameColor( Shape const & ) const;

	Shape const * FindShapeUnderPoint( MicroMeterPoint const, std::function<bool(Shape const &)> const & ) const;
	Shape const * FindShapeUnderPoint( MicroMeterPoint const ) const;

	wchar_t const * const GetParameterLiteral( tParameter const ) const;
	wchar_t const * const GetParameterName   ( tParameter const ) const;
	wchar_t const * const GetParameterUnit   ( tParameter const ) const;

	float const GetParameterValue( tParameter const, Shape const * = nullptr ) const;

	bool HasAxon( ShapeId const id ) const { return GetConstShape( id )->HasAxon( ); }

	template <typename T> bool IsType( ShapeId const id ) const { return T::TypeFits( GetConstShape( id )->GetShapeType() ); }

	bool IsTerminalType ( ShapeId const id ) const { return ::IsTerminalType ( GetConstShape( id )->GetShapeType() ); }
	bool IsStartKnotType( ShapeId const id ) const { return ::IsStartKnotType( GetConstShape( id )->GetShapeType() ); }
	bool IsEndKnotType  ( ShapeId const id ) const { return ::IsEndKnotType  ( GetConstShape( id )->GetShapeType() ); }

	bool IsPointInShape ( Shape const * const pShape, MicroMeterPoint const & point ) const { return pShape->IsPointInShape( point ); }

	bool IsHighlighted     ( Shape const & shape ) const { return shape.GetId() == m_shapeHighlighted; }
	bool IsSuperHighlighted( Shape const & shape ) const { return shape.GetId() == m_shapeSuperHighlighted; }

	int const GetNrOfInputNeurons ( ) const { return m_nrOfInputNeurons; }
	int const GetNrOfOutputNeurons( ) const { return m_nrOfOutputNeurons; }

	bool const HighlightedShapeCanBeDeleted( ) const 
	{
		tShapeType type { GetHighlightedShapeType() };
		if ( type == tShapeType::inputNeuron )
			return m_nrOfInputNeurons > 1;
		else if ( type == tShapeType::outputNeuron )
			return m_nrOfOutputNeurons > 1;
		else 
			return false;
	}

	// manipulating functions

	void ResetSimulationTime( )	{ m_timeStamp = 0._MicroSecs; }

	template <typename T> ShapeId NewShape( MicroMeterPoint const & pos ) { return addShape( new T( this, pos ) ); }

	ShapeId NewPipeline( ShapeId const, ShapeId const );

	void SplitPipeline  ( MicroMeterPoint const & );
	void InsertNeuron   ( MicroMeterPoint const & );
	void AddOutputNeuron( MicroMeterPoint const & );
	void AddInputNeuron ( MicroMeterPoint const & );
	void Connect( );

	void RemoveShape( );

	ShapeId const HighlightShape( ShapeId const id ) 
	{
		ShapeId idOld = m_shapeHighlighted;
		m_shapeHighlighted = id;
		return idOld;
	};

	ShapeId const SuperHighlightShape( ShapeId const id ) 
	{ 
		ShapeId idOld = m_shapeSuperHighlighted;
		m_shapeSuperHighlighted = id;
		return idOld;
	}

	template <typename T>
	void Apply2All( std::function<void(T &)> const & func ) const
	{
		for ( auto pShape : m_Shapes )
		{
			if ( pShape && T::TypeFits( pShape->GetShapeType() ) )
				func( static_cast<T &>( * pShape ) );
		}
	}

	void Apply2AllParameters( std::function<void(tParameter const &)> const & ) const;

	void RecalcPipelines( );

	virtual void CopyModelData( ModelInterface const * const );
	virtual void Compute( );
	virtual void ResetAll( );

	void  const SetParameter( ShapeId const, tParameter const,	float const );

	void IncNrOfInputNeurons () 
	{ 
		++m_nrOfInputNeurons; 
	}
	void IncNrOfOutputNeurons() 
	{ 
		++m_nrOfOutputNeurons; 
	}

	void DecNrOfInputNeurons () 
	{ 
		--m_nrOfInputNeurons; 
	}
	void DecNrOfOutputNeurons() 
	{ 
		--m_nrOfOutputNeurons; 
	}

private:
			  
	// model data
	vector<Shape *> m_Shapes;
	MicroSecs       m_timeStamp;
	int             m_nrOfInputNeurons;
	int             m_nrOfOutputNeurons;

    // used by editor
	ShapeId m_shapeHighlighted;
	ShapeId m_shapeSuperHighlighted;

	// parameters
	float        m_dampingFactor;     // signal loss per um  
    mV           m_threshold;
    mV           m_peakVoltage;   
	MicroSecs    m_pulseWidth;   
	MicroSecs    m_refractoryPeriod;
	meterPerSec  m_pulseSpeed;

	// local functions
	void          deleteBaseKnot( ShapeId const );
	void          deletePipeline( ShapeId const );
	void          checkConsistency( );
	ShapeId const addShape( Shape * );
	void          insertNewBaseKnot( BaseKnot * const );
	void          prepareSplit( MicroMeterPoint const & );
};
