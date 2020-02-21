// NNetModelStorage.h 
//
// NNetModel

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
	NNetModelStorage( NNetModel * const );

	void Write( wostream & );
	bool Read( );

	NNetModel * GetModel( ) { return m_pModel; }

	wstring const GetModelPath  ( ) { return m_wstrPathOfOpenModel; };
	void          ResetModelPath( ) { m_wstrPathOfOpenModel = L""; }

	bool AskSave( );
	bool AskModelFile( );
	bool SaveModel( );
	bool SaveModelAs( );

private:
	NNetModel     * m_pModel;
	wstring         m_wstrPathOfOpenModel;
	vector<ShapeId> m_CompactIds;

	long getCompactIdVal( ShapeId const id ) { return m_CompactIds[ id.GetValue() ].GetValue();	}

	void writeModel( );
	void WriteShape(  wostream &, Shape & );
	void WriteMicroMeterPoint( wostream &, MicroMeterPoint const & );
	void WritePipeline( wostream &, Shape const & );
};
