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

class EventInterface;
class MonitorData;
class Param;

using std::unique_ptr;
using std::move;

class NNetModel
{
public:
	bool operator==( NNetModel const & ) const;

	void Initialize
	( 
		MonitorData * const,
		Param       * const,
		Observable  * const, 
		Observable  * const, 
		Observable  * const 
	);

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

	bool const IsShapeDefined( ShapeId const id ) const
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
	size_t     const GetSizeOfShapeList( ) const { return m_Shapes.Size(); }

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
		ShapeId idNewSlot { Cast2Long(GetSizeOfShapeList()) };
		m_Shapes.AddShape( nullptr );
		return idNewSlot;
	}

	void StaticModelChanged( );

	void SetShapeErrorHandler( ShapeErrorHandler * const pHandler )
	{	
		m_Shapes.SetShapeErrorHandler( pHandler );
	}

	void SelectAllShapes( tBoolOp const op ) { m_Shapes.SelectAllShapes( op ); }

	ShapeId Add2Model( UPShape up ) 
	{ 
		return m_Shapes.AddShape( move(up) ); 
	}

	template <typename T>
	unique_ptr<T> ReplaceInModel( unique_ptr<Shape> up ) 
	{
		ShapeId const id { up.get()->GetId() };
		return move(m_Shapes.ReplaceShape( move(up), id )); 
	}

	template <typename T>
	unique_ptr<T> Store2Model( unique_ptr<T> up ) 
	{
		ShapeId const id { up.get()->GetId() };
		return move(m_Shapes.SetShape( move(up), id )); 
	}

	template <typename T>
	unique_ptr<T> RemoveFromModel( ShapeId const id ) { return m_Shapes.SetShape( unique_ptr<T>(), id ); }

	template <typename T>
	unique_ptr<T> InsertAtModelSlot( unique_ptr<T> up, ShapeId const id )                       
	{ 
		up.get()->SetId( id );
		return m_Shapes.SetShape( move(up), id );
	}

	ShapeList const & GetShapes( ) const { return m_Shapes; }
	ShapeList       & GetShapes( )       { return m_Shapes; }

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
