// Knot.h
//
// NNetModel

#pragma once

#include <vector>
#include "MoreTypes.h"
#include "Pipeline.h"

using std::vector;

class Knot
{
public:
	Knot( MicroMeterPoint const npCenter )
	  : m_npCenter( npCenter ),
		m_potential( 0 )
	{
	}

	MicroMeterPoint GetPosition( ) const
	{
		return m_npCenter;
	}

	void AddIncomming( Pipeline * pPipe )
	{
		m_incomming.push_back( pPipe );
		pPipe->SetEndKnot( this );
	}

	void AddOutgoing( Pipeline * pPipe )
	{
		m_outgoing.push_back( pPipe );
		pPipe->SetStartKnot( this );
	}

	virtual mV GetNextPotential( )
	{
		return m_potential;
	}

protected:
	mV m_potential;

private:

	vector<Pipeline *> m_incomming;
	vector<Pipeline *> m_outgoing;

	MicroMeterPoint m_npCenter;
};