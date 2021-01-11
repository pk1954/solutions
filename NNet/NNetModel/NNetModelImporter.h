// NNetModelImporter.h 
//
// NNetModel

#pragma once

#include <vector>
#include <fstream>
#include <string>
#include "ObserverInterface.h"
#include "MoreTypes.h"
#include "ShapeId.h"
#include "NNetModelStorage.h"
#include "NNetModelWriterInterface.h"

using std::unique_ptr;
using std::wostream;
using std::wstring;
using std::vector;
using std::wstring;

class Param;
class Script;
class Pipe;
class Shape;
class NNetModel;
class ShapeList;
class Observable;
class WrapBase;

class ImportTermination
{
public:
	enum class Result
	{
		ok,
		fileNotFound,
		errorInFile
	};

	virtual void Reaction( ImportTermination::Result const, wstring const ) = 0;
};

class NNetModelImporter
{
public:

	void Initialize( Script * const, ImportTermination * const );

	bool Import( wstring const, bool const );

	unique_ptr<NNetModel> GetImportedModel( );

	wstring const GetFilePath() { return m_wstrFile2Read; }

private:
	NNetModelWriterInterface & getWriterInterface() { return m_ImportedNMWI; }

	unique_ptr<NNetModel>    m_upImportedModel;
	NNetModelWriterInterface m_ImportedNMWI;
	wstring                  m_wstrFile2Read;
	Script                 * m_pScript      { nullptr };
	ImportTermination      * m_pTermination { nullptr };

	void prepareForReading( );
	void readModel( );

	friend static unsigned int __stdcall readModelThreadProc( void * );

	friend WrapBase;
};
