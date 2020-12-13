// Track.h : 
//
// NNetModel

#pragma once

#include <memory>
#include <vector>
#include "Observable.h"
#include "NamedType.h"
#include "SignalInterface.h"

using std::vector;
using std::unique_ptr;

using SignalNr   = NamedType< int, struct SignalNrParam >;
using SignalFunc = function<void(SignalNr const &)>;

class SignalId;

class Track
{
public:
	void CheckSignals( ) const;

	SignalNr     const AddSignal   ( unique_ptr<SignalInterface> );
	unique_ptr<SignalInterface> RemoveSignal( SignalNr const );

	SignalInterface const & GetSignal( SignalNr const ) const;
	bool   const   IsValid  ( SignalNr const ) const;

	void Apply2AllSignals( SignalFunc const & ) const;

private:

	vector<unique_ptr<SignalInterface>> m_signals;
};
