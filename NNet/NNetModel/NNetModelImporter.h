// NNetModelImporter.h 
//
// NNetModel

#pragma once

#include <vector>
#include <fstream>
#include <string>
#include "ObserverInterface.h"
#include "MoreTypes.h"
#include "NobId.h"
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
class Nob;
class NNetModel;
class UPNobList;
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

	virtual ~ImportTermination() {}

	virtual void Reaction(ImportTermination::Result const, wstring const) = 0;
};

class NNetModelImporter
{
public:

	void Initialize();

	bool Import(wstring const &, unique_ptr<ImportTermination>);

	unique_ptr<NNetModel> GetImportedModel();
	UPNobList             GetUPNobs();

	static void CheckImportedNobId(Script &, UPNobList const &, NobId const);

	NNetModelWriterInterface & GetWriterInterface() { return m_ImportedNMWI; }

private:

	unique_ptr<ImportTermination> m_upTermination;
	unique_ptr<NNetModel>         m_upImportedModel;
	NNetModelWriterInterface      m_ImportedNMWI;
	wstring                       m_wstrFile2Read;

	void importModel();

	friend static unsigned int __stdcall importModelThreadProc(void *);

	friend WrapBase;
};
