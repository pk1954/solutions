// TrackNr.ixx
//
// NNetModel

module;

#include <iostream>

export module TrackNr;

import Types;
import Script;

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
