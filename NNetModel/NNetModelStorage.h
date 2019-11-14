// NNetModelStorage.h 
//
// NNetModel

#pragma once

#include <fstream>

using std::wostream;
using std::wstring;

class NNetModel;
class Shape;

class NNetModelStorage
{
public:
	NNetModelStorage( NNetModel * const );

	void Write( wostream & );
	bool Read ( wstring const & wstrPath );

	NNetModel * GetModel( ) { return m_pModel; }

private:
	NNetModel * m_pModel;

	void WriteShape(  wostream &, Shape & );
	void WriteMicroMeterPoint( wostream &, MicroMeterPoint const & );
	void WritePipeline( wostream &, Shape const & );
};
