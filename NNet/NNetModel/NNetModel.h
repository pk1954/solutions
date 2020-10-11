// NNetModel.h 
//
// NNetModel

#pragma once

#include <vector>
#include <exception>
#include "util.h"
#include "BoolOp.h"
#include "MoreTypes.h"
#include "Observable.h"
#include "tParameter.h"
#include "tHighlightType.h"
#include "ShapeErrorHandler.h"
#include "ShapeList.h"
#include "InputNeuron.h"
#include "Neuron.h"
#include "Knot.h"
#include "Pipe.h"

using std::remove_pointer;

class EventInterface;
class MonitorData;
class Param;

class NNetModel
{
public:
	void Initialize
	( 
		MonitorData * const,
		Param       * const,
		Observable  * const, 
		Observable  * const, 
		Observable  * const 
	);

	NNetModel( ) {};

	NNetModel( NNetModel const & );

	virtual ~NNetModel() {}

	// readOnly functions

	void CheckModel( ) const
	{
#ifdef _DEBUG
		m_Shapes.CheckShapeList( );
#endif
	}

	bool const IsPipe( ShapeId const id ) const
	{
		return GetConstShape( id )->GetShapeType().IsPipeType();
	}

	bool const IsShapeNullPtr( ShapeId const id ) const
	{
		return m_Shapes.GetAt( id ) == nullptr;
	}

	Shape const * GetConstShape( ShapeId const id ) const 
	{	
		if ( IsUndefined( id ) || ! m_Shapes.IsValidShapeId( id ) )
		{
			m_Shapes.CallErrorHandler( id );  
			return nullptr;
		}
		return m_Shapes.GetAt( id );
	}

	template <typename T>
	T GetShapeConstPtr( ShapeId const id ) const
	{
		Shape const * const pShape { GetConstShape( id ) };
		return (pShape && HasType<T>( * pShape )) ? static_cast<T>( pShape ) : nullptr;
	}

	fHertz          const GetPulseRate( ShapeId const ) const;
	MicroMeterPoint const GetShapePos ( ShapeId const ) const;

	fMicroSecs const GetSimulationTime ( ) const { return m_timeStamp; }
	long       const GetSizeOfShapeList( ) const { return Cast2Long( m_Shapes.Size() ); }

	BaseKnot * const GetStartKnotPtr(ShapeId const id) const { return GetShapeConstPtr<Pipe const *>(id)->GetStartKnotPtr(); }
	BaseKnot * const GetEndKnotPtr  (ShapeId const id) const { return GetShapeConstPtr<Pipe const *>(id)->GetEndKnotPtr  (); }

	ShapeId const GetStartKnotId(ShapeId const idPipe) const { return GetStartKnotPtr(idPipe)->GetId(); }
	ShapeId const GetEndKnotId  (ShapeId const idPipe) const { return GetEndKnotPtr  (idPipe)->GetId(); }

	// manipulating functions

	void SetSimulationTime( fMicroSecs const newVal = 0._MicroSecs )	
	{ 
		m_timeStamp = newVal; 
		m_pModelTimeObservable->NotifyAll( false );
	}

	virtual bool Compute( );

	ShapeId const FindShapeAt( MicroMeterPoint const &, ShapeCrit const & ) const;

	void  ToggleStopOnTrigger( Neuron * );
	void  RecalcAllShapes( );
	void  ResetModel( );
	float SetParam( tParameter const, float const );
	void  IncShapeList( long const lNrOfShapes ) 
	{ 
		m_Shapes.Resize( m_Shapes.Size() + lNrOfShapes ); 
	}

	MicroMeterRect GetEnclosingRect() const { return m_enclosingRect; }

	void SelectSubtree( BaseKnot * const, tBoolOp const );

	ShapeId const NewShapeListSlot( )
	{
		ShapeId idNewSlot { GetSizeOfShapeList() };
		m_Shapes.Add( nullptr );
		return idNewSlot;
	}

	void StaticModelChanged( );

	void SetShapeErrorHandler( ShapeErrorHandler * const pHandler )
	{	
		m_Shapes.SetShapeErrorHandler( pHandler );
	}

	void SelectAllShapes( tBoolOp const op )                           { m_Shapes.SelectAllShapes( op ); }
	void ReplaceInModel ( Shape * const p2BeReplaced, Shape * pShape ) { m_Shapes.SetShape( pShape,  p2BeReplaced->GetId() ); }
	void Store2Model    ( Shape * const pShape )                       { m_Shapes.SetShape( pShape,  pShape->GetId() ); }
	void RemoveFromModel( Shape * const pShape )                       { m_Shapes.SetShape( nullptr, pShape->GetId() ); }

	void InsertAtModelSlot( Shape * const pShape, ShapeId const id )                       
	{ 
		pShape->SetId( id );
		m_Shapes.SetShape( pShape, id );
	}

	ShapeList const & GetShapes( ) const { return m_Shapes; }

    MonitorData * GetMonitorData( ) { return m_pMonitorData; }

private:

	ShapeList      m_Shapes                  { };
	fMicroSecs     m_timeStamp               { 0._MicroSecs };
	Param        * m_pParam                  { nullptr };
	Observable   * m_pModelTimeObservable    { nullptr };
	Observable   * m_pStaticModelObservable  { nullptr };
	Observable   * m_pDynamicModelObservable { nullptr };
	MonitorData  * m_pMonitorData            { nullptr };
	MicroMeterRect m_enclosingRect           { };

	// local functions

	void incTimeStamp( )
	{
		m_timeStamp += m_pParam->GetTimeResolution( );
		m_pModelTimeObservable->NotifyAll( false );
	}

	void dynamicModelChanged( ) const 
	{ 
		m_pDynamicModelObservable->NotifyAll( false );
	}
};
