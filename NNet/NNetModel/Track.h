// Track.h : 
//
// NNetModel

#pragma once

#include <vector>
#include "Observable.h"
#include "NamedType.h"
#include "Signal.h"

using std::vector;

using SignalNr   = NamedType< int, struct SignalNrParam >;
using SignalIter = vector<Signal *>::const_iterator;
using SignalFunc = function<void(SignalIter const  )>;

class Track
{
public:
	void Clear( );
	void CheckSignals( ) const;

	SignalNr const AddSignal   ( Signal * const );
	Signal * const RemoveSignal( SignalNr const );

	SignalNr       const GetSignalNr( SignalIter const ) const;
	Signal const * const GetSignal  ( SignalNr   const ) const;
	bool           const IsValid    ( SignalNr   const ) const;

	void Apply2AllSignalsC( SignalFunc const & ) const;

private:

	SignalIter getSignalIterator( SignalNr const ) const;

	vector<Signal *> m_signals;
};
