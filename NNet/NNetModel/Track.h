// Track.h : 
//
// NNetModel

#pragma once

#include <memory>
#include <vector>
#include "Observable.h"
#include "NamedType.h"
#include "SignalFactory.h"
#include "Signal.h"

using std::vector;
using std::unique_ptr;

using SignalNr     = NamedType< int, struct SignalNrParam >;
using SignalNrFunc = function<void(SignalNr const &)>;

class SignalId;

class Track
{
public:

	Track()                           = default;  // constructor   
	~Track()                          = default;  // destructor
	Track             (     Track&&) = delete;   // move constructor
	Track & operator= (const Track&) = delete;   // copy assignment operator
	Track & operator= (     Track&&) = delete;   // move assignment operator
	Track             (const Track&);            // copy constructor

	bool operator==(Track const &) const;

	void CheckSignals() const;

	SignalNr     const AddSignal   (unique_ptr<Signal>);
	unique_ptr<Signal> RemoveSignal(SignalNr const);

	Signal const * const GetSignalPtr(SignalNr const) const;
	Signal       * const GetSignalPtr(SignalNr const);
	bool           const IsValid     (SignalNr const) const;
	bool           const IsEmpty     () const { return m_signals.empty(); }

	void Apply2AllSignals(SignalNrFunc const &) const;
	void Apply2AllSignals(SignalFunc   const &) const;

	Signal * const FindSignal(SignalCrit const &);

private:
	vector<unique_ptr<Signal>> m_signals;
};
