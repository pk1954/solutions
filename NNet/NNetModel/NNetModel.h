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
#include "NNetParameters.h"
#include "tHighlightType.h"
#include "MonitorData.h"
#include "ModelDescription.h"
#include "ShapeErrorHandler.h"
#include "ShapeList.h"
#include "InputNeuron.h"
#include "Neuron.h"
#include "Knot.h"
#include "Pipe.h"

class EventInterface;

using std::unique_ptr;
using std::move;

class NNetModel
{
public:
	bool operator==( NNetModel const & ) const;

	void Initialize
	( 
		Observable * const, 
		Observable * const, 
		Observable * const,
		Observable * const 
	);

	// readOnly functions

	template <Shape_t T>
	T GetShapeConstPtr( ShapeId const id ) const
	{
		Shape const * const pShape { GetConstShape( id ) };
		return (pShape && HasType<T>( * pShape )) ? static_cast<T>( pShape ) : nullptr;
	}

	void CheckModel( ) const;
	void DumpModel ( ) const;

	Shape           const * GetConstShape( ShapeId const ) const;
	fHertz          const   GetPulseRate ( ShapeId const ) const;
	MicroMeterPoint const   GetShapePos  ( ShapeId const ) const;

	fMicroSecs const GetSimulationTime ( )             const { return m_timeStamp; }
	size_t     const GetSizeOfShapeList( )             const { return m_Shapes.Size(); }
	float      const GetParameter(tParameter const p)  const { return m_param.GetParameterValue(p); }

	BaseKnot * const GetStartKnotPtr(ShapeId const id) const { return GetShapeConstPtr<Pipe const *>(id)->GetStartKnotPtr(); }
	BaseKnot * const GetEndKnotPtr  (ShapeId const id) const { return GetShapeConstPtr<Pipe const *>(id)->GetEndKnotPtr  (); }

	ShapeId const GetStartKnotId(ShapeId const idPipe) const { return GetStartKnotPtr(idPipe)->GetId(); }
	ShapeId const GetEndKnotId  (ShapeId const idPipe) const { return GetEndKnotPtr  (idPipe)->GetId(); }

	bool    const IsShapeDefined( ShapeId const id )   const { return m_Shapes.IsShapeDefined( id ); }

	// manipulating functions

	virtual bool Compute( );

	ShapeId const FindShapeAt( MicroMeterPoint const &, ShapeCrit const & ) const;

	void  SetSimulationTime( fMicroSecs const newVal = 0._MicroSecs );
	void  ToggleStopOnTrigger( Neuron * );
	void  RecalcAllShapes( );
	void  ResetModel( );
	float SetParam( tParameter const, float const );
	void  SelectSubtree( BaseKnot * const, tBoolOp const );
	void  StaticModelChanged( );

	MicroMeterRect GetEnclosingRect() const { return m_enclosingRect; }

	ShapeId const Push2Model( UPShape upShape ) 
	{ 
		ShapeId idNewSlot { m_Shapes.IdNewSlot( ) };
		m_Shapes.Push( move(upShape) ); 
		return idNewSlot;
	}

	template <Shape_t T>
	unique_ptr<T> PopFromModel( ) { return move(m_Shapes.Pop<T>()); }

	template <Shape_t OLD>
	unique_ptr<OLD> RemoveFromModel( ShapeId const id ) 
	{ 
		Shape * pShape { m_Shapes.RemoveShape( id ) }; 
		return move( unique_ptr<OLD>( static_cast<OLD*>(pShape) ) );
	}

	template <Shape_t NEW, Shape_t OLD>
	unique_ptr<OLD> ReplaceInModel( unique_ptr<NEW> up ) 
	{
		ShapeId const id     { up.get()->GetId() };
		Shape       * pShape { m_Shapes.ReplaceShape( id, move(up) ) }; 
		return move( unique_ptr<OLD>( static_cast<OLD*>(pShape) ) );
	}

	ShapeList   const & GetShapes( )      const { return m_Shapes; }
	ShapeList         & GetShapes( )            { return m_Shapes; }

	MonitorData const & GetMonitorData( ) const { return m_monitorData; }
	MonitorData       & GetMonitorData( )       { return m_monitorData; }

	Param       const & GetParams()       const { return m_param; }
	Param             & GetParams()             { return m_param; }

	wstring const GetModelFilePath() const { return m_wstrModelFilePath; }
	void          SetModelFilePath( wstring const wstr ) { m_wstrModelFilePath = wstr; }

	void AddDescriptionLine( wstring const wstr ) {	m_description.AddDescriptionLine( wstr ); }

	bool const AnyUnsavedChanges( ) const { return m_bUnsavedChanges; }
	void SetUnsavedChanges( bool const bState )
	{
		m_bUnsavedChanges = bState;
		m_pUnsavedChangesObservable->NotifyAll( false );
	}

private:

	ShapeList        m_Shapes                    { };
	fMicroSecs       m_timeStamp                 { 0._MicroSecs };
	Observable     * m_pModelTimeObservable      { nullptr };
	Observable     * m_pStaticModelObservable    { nullptr };
	Observable     * m_pDynamicModelObservable   { nullptr };
	Observable     * m_pUnsavedChangesObservable { nullptr };
	MicroMeterRect   m_enclosingRect             { };
	wstring          m_wstrModelFilePath         { L"" };
	bool             m_bUnsavedChanges           { false }; 
	ModelDescription m_description;
	MonitorData      m_monitorData;
	Param            m_param;

	// local functions

	void incTimeStamp( )
	{
		m_timeStamp += m_param.GetTimeResolution( );
		m_pModelTimeObservable->NotifyAll( false );
	}

	void dynamicModelChanged( ) const 
	{ 
		m_pDynamicModelObservable->NotifyAll( false );
	}
};
