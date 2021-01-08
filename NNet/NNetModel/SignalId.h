// SignalId.h : 
//
// NNetModel

#pragma once

#include "NamedType.h"
#include "script.h"
#include "Track.h"
#include "TrackNr.h"
#include "SignalId.h"

using std::vector;

using SignalIdFunc = function<void(SignalId const &)>;

class SignalId
{
public:
	SignalId( )
	  : trackNr ( TrackNr ::NULL_VAL() ),
		signalNr( SignalNr::NULL_VAL() )
	{ }

	SignalId( TrackNr const tNr, SignalNr const sNr )
	  : trackNr( tNr),
		signalNr( sNr)
	{ }

	void Set2Null() 
	{ 
		trackNr.Set2Null(); 
		signalNr.Set2Null(); 
	}

	bool IsNull   () const { return trackNr.IsNull() || signalNr.IsNull(); }
	bool IsNotNull() const { return trackNr.IsNotNull() && signalNr.IsNotNull(); }

	auto operator== (const SignalId & other) const
	{
		return (trackNr == other.trackNr) && (signalNr == other.signalNr);
	}

	auto operator!= (const SignalId & other) const
	{
		return ! (*this == other);
	}

	TrackNr  const GetTrackNr () const { return trackNr; }
	SignalNr const GetSignalNr() const { return signalNr; }

	friend wostream & operator<< ( wostream & out, SignalId const & id )
	{
		out << L" (" << id.trackNr << L"|" << id.signalNr << L")";
		return out;
	}

private:
	TrackNr  trackNr;
	SignalNr signalNr;
};

inline static const SignalId SignalIdNull { TrackNr::NULL_VAL(), SignalNr::NULL_VAL() };
