// NNetModelStorage.h 
//
// NNetModel

#pragma once

#include <fstream>

using std::wostream;
using std::wistream;

class NNetModel;
class Shape;

void WriteModel( NNetModel const &, wostream & );

void WriteShape( NNetModel const &, wostream &, Shape & );
void WriteMicroMeter( wostream &, MicroMeter const & );
void WriteMicroMeterPoint( wostream &, MicroMeterPoint const & );


void ReadModel ( NNetModel       &, wistream & );
