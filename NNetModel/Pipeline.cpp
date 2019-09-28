// Pipeline.cpp 
//
// NNetModel

#include "stdafx.h"
#include "NNetTypes.h"
#include "Knot.h"
#include "Pipeline.h"

MicroMeter Pipeline::distance( NNetPoint const & npA, NNetPoint const & npB )
{
	NNetPoint  npDiff   = npA - npB;
	MicroMeter distance = MicroMeter
	(
		sqrt
		( 
			npDiff.GetXvalue() * npDiff.GetXvalue() + 
			npDiff.GetYvalue() * npDiff.GetYvalue() 
		)
	);
	return distance;
}

void Pipeline::initialize( )
{
	if ( m_pKnotStart && m_pKnotEnd )
	{
		static double     const IMPULSE_SPEED  = 20.0; // in units of micrometer/microsecond
		static MicroMeter const SEGMENT_LENGTH = MicroMeter( IMPULSE_SPEED * TIME_RESOLUTION.count() );
		
		MicroMeter   const pipelineLength = distance( m_pKnotStart->GetPosition(), m_pKnotEnd->GetPosition() );
		unsigned int const iNrOfSegments  = CastToUnsignedInt(round(pipelineLength.GetValue() / SEGMENT_LENGTH.GetValue()));
		m_potential.resize( iNrOfSegments, BASE_POTENTIAL );
	}
}

void Pipeline::SetStartKnot( Knot * pKnot )
{
	m_pKnotStart = pKnot;
	initialize();
}

void Pipeline::SetEndKnot( Knot * pKnot )
{
	m_pKnotEnd = pKnot;
	initialize();
}

NNetPoint Pipeline::GetStartPoint( ) const 
{ 
	return m_pKnotStart ? m_pKnotStart->GetPosition() : NNetPoint::NULL_VAL(); 
}

NNetPoint Pipeline::GetEndPoint( ) const 
{ 
	return m_pKnotEnd ? m_pKnotEnd->GetPosition() : NNetPoint::NULL_VAL();
}

MicroMeter Pipeline::GetWidth( ) const 
{ 
	return m_width; 
}

void Pipeline::Step()
{
	for (std::size_t i = m_potential.size() - 1; i > 0; --i )
	{
		m_potential[i] = m_potential[i-1];
	}

	m_potential[0] = m_pKnotStart->GetNextPotential();
}

bool Pipeline::GetSegment( int const iNr, Segment & seg, mV & potential ) const
{
	if ( iNr < m_potential.size() )
	{
		NNetPoint npDiff = GetEndPoint() - GetStartPoint();
		seg = Segment
		{
			GetStartPoint() + (npDiff *  iNr     ) / static_cast<int>(m_potential.size()),
			GetStartPoint() + (npDiff * (iNr + 1)) / static_cast<int>(m_potential.size()),
			m_width
		};
		potential = m_potential[ iNr ];
		assert ( potential < 200.0_mV );
		return true;
	}
	else 
		return false;
}
