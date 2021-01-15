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
#include "ParameterType.h"
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
	// const functions

	bool operator==( NNetModel const & ) const;

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

	fMicroSecs const GetSimulationTime ( ) const { return m_timeStamp; }

	float const GetParameter(ParameterType::Value const p) const { return m_param.GetParameterValue(p); }

	BaseKnot * const GetStartKnotPtr(ShapeId const id) const { return GetShapeConstPtr<Pipe const *>(id)->GetStartKnotPtr(); }
	BaseKnot * const GetEndKnotPtr  (ShapeId const id) const { return GetShapeConstPtr<Pipe const *>(id)->GetEndKnotPtr  (); }

	ShapeId const GetStartKnotId(ShapeId const idPipe) const { return GetStartKnotPtr(idPipe)->GetId(); }
	ShapeId const GetEndKnotId  (ShapeId const idPipe) const { return GetEndKnotPtr  (idPipe)->GetId(); }

	ShapeList        const & GetShapes( )       const { return m_Shapes; }
	MonitorData      const & GetMonitorData( )  const { return m_monitorData; }
	ModelDescription const & GetDescription( )  const { return m_description; }
	Param            const & GetParams()        const { return m_param; }
	wstring          const   GetModelFilePath() const { return m_wstrModelFilePath; }

	ShapeId const FindShapeAt( MicroMeterPoint const &, ShapeCrit const & ) const;

	// non const functions

	virtual bool Compute( );

	void  RecalcAllShapes( );
	void  ResetModel( );
	float SetParam( ParameterType::Value const, float const );
	void  SelectSubtree( BaseKnot * const, tBoolOp const );

	ShapeList   & GetShapes()      { return m_Shapes; }
	MonitorData & GetMonitorData() { return m_monitorData; }
	Param       & GetParams()      { return m_param; }

	void SetModelFilePath  ( wstring const wstr ) { m_wstrModelFilePath = wstr; }
	void AddDescriptionLine( wstring const wstr ) {	m_description.AddDescriptionLine( wstr ); }
	void SetSimulationTime( fMicroSecs const newVal = 0._MicroSecs ) { m_timeStamp = newVal; }

private:

	ShapeList        m_Shapes;
	ModelDescription m_description;
	MonitorData      m_monitorData;
	Param            m_param;
	fMicroSecs       m_timeStamp         { 0._MicroSecs };
	wstring          m_wstrModelFilePath { L"" };
};
