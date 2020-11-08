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

	void DumpModel( ) const;

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
			DumpModel();
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

	template <typename T>
	unique_ptr<T> PopFromModel( ) { return move(m_Shapes.Pop<T>()); }

	void StaticModelChanged( );

	void SetShapeErrorHandler( ShapeErrorHandler * const pHandler )
	{	
		m_Shapes.SetShapeErrorHandler( pHandler );
	}

	void SelectAllShapes( tBoolOp const op ) { m_Shapes.SelectAllShapes( op ); }

	ShapeId const Push2Model( UPShape upShape ) 
	{ 
		ShapeId idNewSlot { m_Shapes.IdNewSlot( ) };
		m_Shapes.Push( move(upShape) ); 
		return idNewSlot;
	}

	template <typename OLD>
	unique_ptr<OLD> RemoveFromModel( ShapeId const id ) 
	{ 
		Shape * pShape { m_Shapes.RemoveShape( id ) }; 
		return move( unique_ptr<OLD>( static_cast<OLD*>(pShape) ) );
	}

	template <typename NEW, typename OLD>
	unique_ptr<OLD> ReplaceInModel( unique_ptr<NEW> up ) 
	{
		ShapeId const id     { up.get()->GetId() };
		Shape       * pShape { m_Shapes.ReplaceShape( id, move(up) ) }; 
		return move( unique_ptr<OLD>( static_cast<OLD*>(pShape) ) );
	}

	void SetInModel( ShapeId const id, UPShape upShape )
	{
		m_Shapes.SetShape2Slot( id, move(upShape) );
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
