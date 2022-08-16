// SignalId.h : 
//
// NNetModel

#pragma once

#include <ostream>
#include <functional>
#include "Track.h"
#include "TrackNr.h"

import NamedType;
import IoConstants;

using std::vector;
using std::function;
using std::wostream;

class SignalId
{
public:
	using Func = function<void(SignalId const &)>;

	SignalId()
	  : trackNr (TrackNr::NULL_VAL()),
		signalNr(SignalNr::NULL_VAL())
	{ }

	SignalId(TrackNr const tNr, SignalNr const sNr)
	  : trackNr(tNr),
		signalNr(sNr)
	{ }

	void Set2Null() 
	{ 
		trackNr.Set2Null(); 
		signalNr.Set2Null(); 
	}

	static SignalId const & NULL_VAL() 
	{ 
		static SignalId const res { SignalId(TrackNr::NULL_VAL(), SignalNr::NULL_VAL()) }; 
		return res;
	};

	bool IsNull () const { return trackNr.IsNull() || signalNr.IsNull(); }
	bool IsValid() const { return trackNr.IsNotNull() && signalNr.IsNotNull(); }

	bool operator== (const SignalId & other) const = default;

	TrackNr  GetTrackNr () const { return trackNr; }
	SignalNr GetSignalNr() const { return signalNr; }

	friend wostream & operator<< (wostream & out, SignalId const & id)
	{
		out << L" (" << id.trackNr << SEPARATOR << id.signalNr << L")";
		return out;
	}

private:
	TrackNr  trackNr;
	SignalNr signalNr;
};
