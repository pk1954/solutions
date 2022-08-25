// TrackNr.ixx
//
// NNetModel

module;

#include <iostream>
#include "Script.h"

export module TrackNr;

import NamedType;

using std::wostream;

export using TrackNr = NamedType< int, struct TrackNrParam >;

export TrackNr ScrReadTrackNr(Script& script)
{
	script.ScrReadString(L"track");
	TrackNr const trackNr{ script.ScrReadInt() };
	return trackNr;
}

export void WriteTrackNr(wostream & out, TrackNr const trackNr)
{
	out << L" track " << trackNr;
}
