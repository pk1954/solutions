// SignalId.h : 
//
// NNetModel

#pragma once

#include "NamedType.h"
#include "Track.h"

using std::vector;

using TrackNr = NamedType< int, struct TrackNrParam >;

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

	auto operator== (const SignalId & other) const
	{
		return (trackNr == other.trackNr) && (signalNr == other.signalNr);
	}

	auto operator!= (const SignalId & other) const
	{
		return ! (*this == other);
	}

	static const SignalId NULL_VAL;

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
