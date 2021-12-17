// SignalId.h : 
//
// NNetModel

#pragma once

#include "NamedType.h"
#include "SCRIPT.H"
#include "Track.h"
#include "TrackNr.h"
#include "SignalId.h"

using std::vector;

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

	bool IsNull   () const { return trackNr.IsNull() || signalNr.IsNull(); }
	bool IsNotNull() const { return trackNr.IsNotNull() && signalNr.IsNotNull(); }

	bool operator== (const SignalId & other) const = default;

	TrackNr  GetTrackNr () const { return trackNr; }
	SignalNr GetSignalNr() const { return signalNr; }

	friend wostream & operator<< (wostream & out, SignalId const & id)
	{
		out << L" (" << id.trackNr << L"|" << id.signalNr << L")";
		return out;
	}

	inline static wchar_t const SEPARATOR     { L'|' };
	inline static wchar_t const OPEN_BRACKET  { L'(' };
	inline static wchar_t const CLOSE_BRACKET { L')' };

private:
	TrackNr  trackNr;
	SignalNr signalNr;
};
