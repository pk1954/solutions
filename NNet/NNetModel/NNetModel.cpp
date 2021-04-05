// NNetModel.cpp 
//
// NNetModel

#include "stdafx.h"
#include "scanner.h"
#include <vector>
#include <unordered_map>
#include "MoreTypes.h"
#include "tHighlightType.h"
#include "ShapeType.h"
#include "Knot.h"
#include "Neuron.h"
#include "InputNeuron.h"
#include "OutputNeuron.h"
#include "NNetModel.h"

using namespace std::chrono;
using std::unordered_map;
using std::wcout;
using std::endl;

bool NNetModel::operator==( NNetModel const & rhs ) const
{
	return
	(m_Shapes            == rhs.m_Shapes            ) &&
	(m_timeStamp         == rhs.m_timeStamp         ) &&
	(m_wstrModelFilePath == rhs.m_wstrModelFilePath ) &&
//	(m_description       == rhs.m_description       ) &&  // not neccessary
	(m_monitorData       == rhs.m_monitorData       ) &&
	(m_param             == rhs.m_param             );
}

void NNetModel::CheckModel( ) const
{
#ifdef _DEBUG
	m_Shapes.CheckShapeList( );
#endif
}

Shape const * NNetModel::GetConstShape( ShapeId const id ) const 
{	
	if ( IsUndefined( id ) || ! m_Shapes.IsValidShapeId( id ) )
	{
		wcout << L"# **** GetConstShape failed. Id = " << id << endl;
		DumpModel();
		m_Shapes.CallErrorHandler( id );  
		return nullptr;
	}
	return m_Shapes.GetAt( id );
}

void NNetModel::RecalcAllShapes( ) 
{ 
	m_Shapes.Apply2All( [&]( Shape & shape ) { shape.Recalc( ); } );
} 

fHertz const NNetModel::GetPulseRate( ShapeId const id ) const
{
	InputNeuron const * const pInputNeuron { GetShapeConstPtr<InputNeuron const *>( id ) };
	return ( pInputNeuron )
		   ? pInputNeuron->GetPulseFrequency( )
	       : fHertz::NULL_VAL();
}

bool const NNetModel::GetDescriptionLine( int const iLine, wstring & wstrLine ) const
{
	return m_description.GetDescriptionLine( iLine, wstrLine );
}

float NNetModel::SetParam
( 
	ParamType::Value const param, 
	float      const fNewValue 
)
{
	float fOldValue { m_param.GetParameterValue( param ) };
	m_param.SetParameterValue( param, fNewValue );
	RecalcAllShapes( );
	return fOldValue;
}

bool NNetModel::Compute( )
{
	bool bStop {false};
	m_timeStamp += m_param.GetTimeResolution();
	m_Shapes.Apply2All( [&](Shape &s) { s.Prepare(); } );
	m_Shapes.Apply2All( [&](Shape &s) { if ( s.CompStep() ) bStop = true; } );
	return bStop;
}

void NNetModel::ResetModel( )
{
	m_wstrModelFilePath = L""; 
	m_Shapes.Clear();
	m_monitorData.Reset();
	m_description.ClearDescription();
	SetSimulationTime();
}

void NNetModel::SelectSubtree(BaseKnot * const pBaseKnot, bool const bOn)
{
	if ( pBaseKnot )
	{
		pBaseKnot->Select(bOn, false);
		pBaseKnot->m_connections.Apply2AllOutPipes
		( 
			[&]( Pipe & pipe ) 
			{ 
				pipe.Select(bOn, false); 
				if ( pipe.GetEndKnotPtr()->IsKnot() )
					SelectSubtree(pipe.GetEndKnotPtr(), bOn); 
			} 
		);
	}
}

ShapeId const NNetModel::FindShapeAt
( 
	MicroMeterPoint const & umPoint, 
	ShapeCrit       const & crit 
) const
{	
	ShapeId idRes { NO_SHAPE };

	if ( idRes == NO_SHAPE )   // first test all connectors
		idRes = m_Shapes.FindShapeAt(umPoint, [&](Shape const & s) { return s.IsConnector() && crit(s); });

	if ( idRes == NO_SHAPE )   // if nothing found, test all typesof neurons
		idRes = m_Shapes.FindShapeAt(umPoint, [&](Shape const & s) { return s.IsAnyNeuron() && crit(s); });

	if ( idRes == NO_SHAPE )   // if nothing found, test knot shapes								   
		idRes = m_Shapes.FindShapeAt(umPoint, [&](Shape const & s) { return s.IsKnot     () && crit(s); }); 	
																									   
	if ( idRes == NO_SHAPE )   // if nothing found, try pipes										   
		idRes = m_Shapes.FindShapeAt(umPoint, [&](Shape const & s) { return s.IsPipe     () && crit(s); });

	return idRes;
}

void NNetModel::DumpModel( ) const
{
	wcout << Scanner::COMMENT_SYMBOL << L"------------ Dump start ------------" << endl;
	m_Shapes.Dump();
	wcout << Scanner::COMMENT_SYMBOL << L"------------ Dump end ------------" << endl;
}
