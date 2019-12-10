// NNetModelStorage.h 
//
// NNetModel

#pragma once

#include <vector>
#include <fstream>
#include "Shape.h"
#include "MoreTypes.h"

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
	bool Read ( wstring const & wstrPath );

	NNetModel * GetModel( ) { return m_pModel; }

	void OpenModel( );
	void SaveModel( );

private:
	NNetModel     * m_pModel;
	wstring         m_wstrPathOfOpenModel;
	vector<ShapeId> m_CompactIds;

	long getCompactIdVal( ShapeId const id )
	{
		return m_CompactIds[ id.GetValue() ].GetValue();
	}

	void WriteShape(  wostream &, Shape & );
	void WriteMicroMeterPoint( wostream &, MicroMeterPoint const & );
	void WritePipeline( wostream &, Shape const & );
};
