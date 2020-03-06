// NNetModelStorage.h 
//
// NNetSimu

#pragma once

#include <vector>
#include <fstream>
#include "MoreTypes.h"
#include "ShapeId.h"

using std::wostream;
using std::wstring;
using std::vector;

class NNetModel;
class Shape;

class NNetModelStorage
{
public:
	void Write( NNetModel const &, wostream & );
	bool Read( NNetModel &, wstring const = L"" );

	wstring const GetModelPath  ( ) { return m_wstrPathOfOpenModel; };
	void          ResetModelPath( );

	int  AskSave( );
	bool AskAndSave( NNetModel const & model );
	bool AskModelFile( );
	bool SaveModel  ( NNetModel const & );
	bool SaveModelAs( NNetModel const & );

private:
	bool            m_bPreparedForReading { false };
	wstring         m_wstrPathOfOpenModel { L"" };
	vector<ShapeId> m_CompactIds;

	long getCompactIdVal( ShapeId const id ) { return m_CompactIds[ id.GetValue() ].GetValue();	}

	void prepareForReading( NNetModel * const );
	void writeModel( NNetModel const & );
	void WriteShape(  wostream &, Shape & );
	void WriteMicroMeterPoint( wostream &, MicroMeterPoint const & );
	void WritePipeline( wostream &, Shape const & );
};
