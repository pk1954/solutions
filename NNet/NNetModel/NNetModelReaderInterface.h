// NNetModelReaderInterface.h 
//
// NNetModel
//
// Interface for reader threads

#pragma once

#include "MoreTypes.h"
#include "ShapeType.h"
#include "ShapeId.h"
#include "NNetModel.h"
#include "SoundInterface.h"
#include "tHighlightType.h"

class Shape;

class NNetModelReaderInterface
{
public:

	void Start( NNetModel * const pModel ) { m_pModel = pModel;	 }
	void Stop ( )                          { m_pModel = nullptr; }

	void DumpModel( ) const { m_pModel->DumpModel(); }
	void CheckModel() const { m_pModel->CheckModel(); };

	bool            const IsConnectionCandidate     ( ShapeId const, ShapeId const ) const;
	bool            const CanConnectTo              ( ShapeId const, ShapeId const ) const;
	bool            const IsConnectedTo             ( ShapeId const, ShapeId const ) const;
	bool            const IsSelected                ( ShapeId const ) const;
	ShapeType       const GetShapeType              ( ShapeId const ) const;
	fHertz          const GetPulseFrequency         ( ShapeId const ) const;
	size_t          const GetNrOfSegments           ( ShapeId const ) const;
	SoundDescr      const GetTriggerSound           ( ShapeId const ) const;
	bool            const HasIncoming               ( ShapeId const ) const;
	bool            const HasOutgoing               ( ShapeId const ) const;
	size_t          const GetNrOfOutgoingConnections( ShapeId const ) const;
	size_t          const GetNrOfIncomingConnections( ShapeId const ) const;
	mV              const GetVoltage                ( ShapeId const ) const;
	mV              const GetVoltage                ( ShapeId const, MicroMeterPoint const & ) const;
			        
	UPShapeList     const & GetUPShapes()                              const { return m_pModel->GetUPShapes(); }
	bool            const   AnyShapesSelected( )                       const { return m_pModel->GetUPShapes().AnyShapesSelected( ); }
	bool            const   IsValidShapeId( ShapeId const id )         const { return m_pModel->GetUPShapes().IsValidShapeId  (id); }
	MicroMeterPoint const   GetShapePos   ( ShapeId const id )         const { return m_pModel->GetShapePos                 (id); }
	Shape           const * GetConstShape ( ShapeId const id )         const { return m_pModel->GetConstShape               (id); }
	size_t          const   GetSizeOfShapeList( )                      const { return m_pModel->GetUPShapes().Size(); }
	fMicroSecs      const   GetSimulationTime( )                       const { return m_pModel->GetSimulationTime ( ); }
	MonitorData     const & GetMonitorData( )                          const { return m_pModel->GetMonitorData    ( ); }
	fMicroSecs      const   GetTimeResolution( )                       const { return m_pModel->GetParams().GetTimeResolution(); };
	wstring         const   GetModelFilePath()                         const { return m_pModel->GetModelFilePath(); }
	float           const   GetParameter(ParameterType::Value const p) const { return m_pModel->GetParameter( p ); }

	bool    const GetDescriptionLine( int const, wstring & ) const;

	ShapeId const FindShapeAt
	( 
		MicroMeterPoint const &, 
		ShapeCrit       const & = ShapeCritAlwaysTrue 
	) const;

	void DrawExterior  ( ShapeId const, DrawContext const &, tHighlightType const ) const;
	void DrawInterior  ( ShapeId const, DrawContext const &, tHighlightType const ) const;
	void DrawNeuronText( ShapeId const, DrawContext const & ) const;
	
	void DrawLine( MicroMeterLine const &, DrawContext const & ) const;

	template <Shape_t T> unsigned long const GetNrOf ( )                  const { return T::GetCounter( ); }
	template <Shape_t T> bool          const IsOfType( ShapeId const id ) const { return T::TypeFits( GetShapeType( id ) ); }

private:
	NNetModel const * m_pModel;

	bool const isConnectedToPipe( ShapeId const, ShapeId const ) const;
	bool const onlyOneAxon( ShapeId const idSrc, ShapeId const idDst ) const
	{
		return (GetNrOfOutgoingConnections(idSrc) + GetNrOfOutgoingConnections(idDst) <= 1);
	}
};
