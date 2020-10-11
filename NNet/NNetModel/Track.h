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
using SignalFunc = function<void(SignalNr const &)>;

struct SignalId;

class Track
{
public:
	void Clear( );
	void CheckSignals( ) const;

	SignalNr const AddSignal   ( Signal * const );
	Signal * const DeleteSignal( SignalNr const );

	Signal const * const GetSignal( SignalNr const ) const;
	bool           const IsValid  ( SignalNr const ) const;

	void Apply2AllSignals( SignalFunc const & ) const;

private:

	vector<Signal *> m_signals;
};
