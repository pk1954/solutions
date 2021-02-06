// AlignCommand.h
//
// Commands

#pragma once

#include <vector>
#include "MoreTypes.h"
#include "LineType.h"
#include "NNetModelWriterInterface.h"
#include "Command.h"
#include "MicroMeterPointVector.h"
#include "UPShapeList.h"
#include "BaseKnot.h"

using std::vector;
using std::unique_ptr;
using std::make_unique;

//class AlignCommand : public Command
//{
//public:
//	AlignCommand( NNetModelWriterInterface & nmwi )
//	{
//		struct AL_PNT
//		{
//			BaseKnot * pBaseKnot;
//			MicroMeter umDist;
//		};
//		vector<AL_PNT> m_points;
//
//		// make lokal copy of selection, sorting out pipes
//		nmwi.GetUPShapes().Apply2All<Shape>
//		( 
//			[&](Shape & s)
//			{ 
//				if ( s.IsSelected() && s.IsAnyNeuron() )
//					m_points.push_back( AL_PNT{ static_cast<BaseKnot *>(&s), 0.0_MicroMeter } );
//			} 
//		);
//
//		if ( m_points.size() <= 3 )
//			return;
//
//		// find 2 baseknots (START/END) with maximum distance
//
//		MicroMeter umMaxDist { 0.0_MicroMeter };
//		for ( auto & it1 : m_points )
//		for ( auto & it2 : m_points )
//		{
//			MicroMeter umDist { Distance( it1.pBaseKnot->GetPosition(), it2.pBaseKnot->GetPosition() ) };
//			if ( umDist > umMaxDist )
//			{
//				umMaxDist = umDist;
//				m_pStart = it1.pBaseKnot;
//				m_pEnd   = it2.pBaseKnot;
//			}
//		}
//
//		// for every other baseknot compute position on line START to END
//
//		LineType<MicroMeter> line      { m_pStart->GetPosition(), m_pEnd->GetPosition() };
//		LineType<MicroMeter> orthoLine { m_pStart->GetPosition(), line.OrthoVector() };
//		for ( auto & it : m_points )
//			it.umDist = PointToLine( orthoLine, it.pBaseKnot->GetPosition() );
//
//		// sort baseknots according to position
//
//		sort( m_points.begin(), m_points.end(), [](auto p1, auto p2){ return p1.umDist < p2.umDist; } );
//
//		for ( auto & it : m_points )
//			m_origin.Add( *(it.pBaseKnot) );
//		
//		// compute tightly packed positions
//
//		MicroMeter      umOriginLength    { line.Length() };
//		MicroMeter      umTargetLength    { NEURON_RADIUS * 2.0f * Cast2Float(m_points.size()) };
//		MicroMeterPoint umPntPackedSingle { line.GetVector() * (umOriginLength / umTargetLength) };
//		MicroMeterPoint umPntTargetCenter { (m_pEnd->GetPosition() - m_pStart->GetPosition()) * 0.5f };
//		MicroMeterPoint umPntTargetStart  { umPntTargetCenter - umPntPackedSingle * (m_points.size() * 0.5f) };
//		MicroMeterPoint umPntTargetEnd    { umPntTargetCenter + umPntPackedSingle * (m_points.size() * 0.5f) };
//
//		//for ( int i = 0; i < m_points.size(); ++i )
//		//	m_target.Add( umPntTargetStart + umPntPackedSingle * static_cast<float>(i) );
//
//	}
//
//	virtual void Do( NNetModelWriterInterface & nmwi ) 
//	{ 
//	}
//
//	virtual ShapeId const GetMovedShape( ) const
//	{
//		return ShapeId();
//	}
//
//private:
//
//	BaseKnot     * m_pStart { nullptr };
//	BaseKnot     * m_pEnd   { nullptr };
//
//	MicroMeterPointVector m_origin;
//	MicroMeterPointVector m_target;
//};
