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

Signal * const Track::RemoveSignal( SignalNr const signalNr )
{
	Signal * pSignal { nullptr };
	if ( IsValid( signalNr ) )
	{
		SignalIter itSignal { getSignalIterator( signalNr ) };
		pSignal = * itSignal;
		m_signals.erase( itSignal );
	}
	return pSignal;
}

void Track::Apply2AllSignalsC( SignalFunc const & func ) const
{
	for ( SignalIter itSignal = m_signals.begin(); itSignal != m_signals.end(); ++itSignal )
	{
		assert( *itSignal != nullptr );
		func( itSignal ); 
	}
}             

Signal const * const Track::GetSignal( SignalNr const signalNr ) const
{
	return IsValid( signalNr ) ? (*getSignalIterator(signalNr)) : nullptr;
}

SignalNr const Track::GetSignalNr( SignalIter const itSignal ) const
{
	return SignalNr( Cast2Int(distance( m_signals.begin(), itSignal )) );
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

SignalIter Track::getSignalIterator( SignalNr const signalNr ) const 
{ 
	return m_signals.begin() + signalNr.GetValue(); 
}
