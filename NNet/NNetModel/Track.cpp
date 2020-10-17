// Track.cpp 
//
// NNetModel

#include "stdafx.h"
#include "Track.h"

using std::move;

SignalNr const Track::AddSignal( unique_ptr<Signal> pSignal )
{
	m_signals.push_back( move(pSignal) );
	return SignalNr( Cast2Int(m_signals.size() - 1) );
}

unique_ptr<Signal> Track::RemoveSignal( SignalNr const signalNr )
{
	if ( IsValid( signalNr ) )
	{
		vector<unique_ptr<Signal>>::iterator itSignal { m_signals.begin() + signalNr.GetValue() };
		unique_ptr<Signal> pSignal { move(*itSignal) };
		m_signals.erase( itSignal );
		return move(pSignal);
	}
	return nullptr;
}

void Track::Apply2AllSignals( SignalFunc const & func ) const
{
	for ( int i = 0; i < m_signals.size(); ++i )
		func( SignalNr( i) ); 
}             

Signal const & Track::GetSignal( SignalNr const signalNr ) const
{
	assert( IsValid( signalNr ) );
	return * m_signals[signalNr.GetValue()].get();
}

bool const Track::IsValid( SignalNr const signalNr ) const
{
	return (0 <= signalNr.GetValue()) && (signalNr.GetValue() < m_signals.size());
}

void Track::CheckSignals( ) const
{
#ifdef _DEBUG
	for (const auto & pSignal : m_signals )
	{
		assert( pSignal != nullptr );
		pSignal->CheckSignal();
	}
#endif
}
