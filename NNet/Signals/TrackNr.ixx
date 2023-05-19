// TrackNr.ixx
//
// Signals

module;

#include <iostream>

export module Signals:TrackNr;

import Types;
import Script;

using std::wostream;

export using TrackNr = NamedType< int, struct TrackNrParam >;

export TrackNr ScrReadTrackNr(Script & script)
{
	script.ScrReadString(L"track");
	TrackNr const trackNr{ script.ScrReadInt() };
	return trackNr;
}
