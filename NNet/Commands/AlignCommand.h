// AlignCommand.h
//
// Commands

#pragma once

#include <vector>
#include "MoreTypes.h"
#include "LineType.h"
#include "NNetModelWriterInterface.h"
#include "Command.h"
#include "BaseKnot.h"

using std::vector;

class AlignCommand : public Command
{
public:
	AlignCommand( NNetModelWriterInterface & nmwi )
	{
		// make lokal copy of selection, sorting out pipes
		nmwi.GetShapes().Apply2All<Shape>
		( 
			[&](Shape & s)
			{ 
				if ( s.IsSelected() && s.IsBaseKnot() )
					m_list.push_back( static_cast<BaseKnot *>(&s) );
			} 
		);

		if ( m_list.size() <= 3 )
			return;

		// find 2 baseknots (START/END) with maximum distance

		MicroMeter umMaxDist { 0.0_MicroMeter };
		for ( auto it1 : m_list )
		for ( auto it2 : m_list )
		{
			MicroMeter umDist { Distance( it1->GetPosition(), it2->GetPosition() ) };
			if ( umDist > umMaxDist )
			{
				umMaxDist = umDist;
				m_pStart = it1;
				m_pEnd   = it2;
			}
		}

		// for every other baseknot compute position on line START to END

		LineType<MicroMeter> line      { m_pStart->GetPosition(), m_pEnd->GetPosition() };
		LineType<MicroMeter> orthoLine { m_pStart->GetPosition(), line.OrthoVector() };
		vector<MicroMeter> vDist;
		for ( auto it : m_list )
		{
			MicroMeter umOrthoDist { PointToLine( orthoLine, it->GetPosition() ) };
			vDist.push_back( umOrthoDist );
		}

		// sort baseknots according to position

		sort( vDist.begin(), vDist.end() );

		// compute tightly packed positions

		MicroMeter      umUnpackedLength  { line.Length() };
		MicroMeter      umPackedLength    { NEURON_RADIUS * 2.0f * Cast2Float(m_list.size()) };
		MicroMeterPoint umPntPackedSingle { line.GetVector() * (umUnpackedLength / umPackedLength) };
		MicroMeterPoint umPntCenter       { (m_pEnd->GetPosition() - m_pStart->GetPosition()) * 0.5f };
		MicroMeterPoint umPntNewStart     { umPntCenter - umPntPackedSingle * (m_list.size() * 0.5f) };
		MicroMeterPoint umPntNewEnd       { umPntCenter + umPntPackedSingle * (m_list.size() * 0.5f) };

	}

	virtual void Do( NNetModelWriterInterface & nmwi ) 
	{ 
	}

	virtual ShapeId const GetMovedShape( ) const
	{
		return ShapeId();
	}

private:

	BaseKnot         * m_pStart { nullptr };
	BaseKnot         * m_pEnd   { nullptr };
	vector<BaseKnot *> m_list;
};
