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

	Track()                                 = default;  // constructor   
	~Track()                                = default;  // destructor
	Track( Track&& rhs )                    = delete;   // move constructor
	Track & operator= ( const Track&  rhs ) = delete;   // copy assignment operator
	Track & operator= (       Track&& rhs ) = delete;   // move assignment operator

	Track( const Track & rhs ) // copy constructor
	{
		for ( auto const & upSignal : m_signals )
			AddSignal( move(SignalFactory::MakeSignal(*upSignal.get())) );
	}

	bool operator==( Track const & rhs ) const
	{
		return m_signals == rhs.m_signals;
	}

	void CheckSignals( ) const;

	SignalNr     const AddSignal   ( unique_ptr<Signal> );
	unique_ptr<Signal> RemoveSignal( SignalNr const );

	Signal const & GetSignal( SignalNr const ) const;
	Signal       & GetSignal( SignalNr const );
	bool   const   IsValid  ( SignalNr const ) const;

	void Apply2AllSignals( SignalNrFunc const & ) const;
	void Apply2AllSignals( SignalFunc   const & );
	Signal * const FindSignal( SignalCrit const & );

private:
	vector<unique_ptr<Signal>> m_signals;
};
