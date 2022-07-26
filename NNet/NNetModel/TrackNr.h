// TrackNr.h : 
//
// NNetModel

#pragma once

#include <iostream>
#include "Script.h"

using std::wostream;

using TrackNr = NamedType< int, struct TrackNrParam >;

static TrackNr ScrReadTrackNr(Script & script)
{
	script.ScrReadString(L"track");
	TrackNr const trackNr { script.ScrReadInt() };
	return trackNr;
}

static void WriteTrackNr(wostream & out, TrackNr const trackNr)
{
	out << L" track " << trackNr;
}
