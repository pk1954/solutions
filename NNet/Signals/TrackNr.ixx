// TrackNr.ixx
//
// Signals

export module Signals:TrackNr;

import std;
import Types;
import RunTime;

using std::wostream;

export using TrackNr = NamedType< int, struct TrackNrParam >;

export TrackNr ScrReadTrackNr(Script & script)
{
	script.ScrReadString(L"track");
	TrackNr const trackNr{ script.ScrReadInt() };
	return trackNr;
}
