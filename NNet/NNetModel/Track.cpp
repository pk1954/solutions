// Track.cpp 
//
// NNetModel

#include "stdafx.h"
#include "Track.h"

void Track::Clear( )
{
	m_signals.clear();
}

SignalNr const Track::AddSignal( Signal * const pSignal )
{
	m_signals.push_back( pSignal );
	return SignalNr( Cast2Int(m_signals.size() - 1) );
}

Signal * const Track::DeleteSignal( SignalNr const signalNr )
{
	Signal * pSignal { nullptr };
	if ( IsValid( signalNr ) )
	{
		vector<Signal *>::const_iterator itSignal { m_signals.begin() + signalNr.GetValue() };
		pSignal = * itSignal;
		m_signals.erase( itSignal );
	}
	return pSignal;
}

void Track::Apply2AllSignals( SignalFunc const & func ) const
{
	for ( int i = 0; i < m_signals.size(); ++i )
		func( SignalNr( i) ); 
}             

Signal const * const Track::GetSignal( SignalNr const signalNr ) const
{
	return IsValid( signalNr ) ? m_signals[signalNr.GetValue()] : nullptr;
}

bool const Track::IsValid( SignalNr const signalNr ) const
{
	return (0 <= signalNr.GetValue()) && (signalNr.GetValue() < m_signals.size());
}

void Track::CheckSignals( ) const
{
#ifdef _DEBUG
	for (auto pSignal : m_signals )
	{
		assert( pSignal != nullptr );
		pSignal->CheckSignal();
	}
#endif
}
