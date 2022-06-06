// NNetModelIO.h 
//
// ModelIO

#pragma once

#include <string>
#include "NobId.h"
#include "NobIdList.h"
#include "ImportTermination.h"
#include "NNetWrapperBase.h"
#include "NNetModelWriterInterface.h"

using std::unique_ptr;
using std::wstring;

class Script;
class NNetModel;
class WrapBase;
class ImportTermination;

class NNetModelIO
{
public:
	~NNetModelIO();

	void Initialize();

	/// import ///

	bool Import(wstring const &, unique_ptr<ImportTermination>);

	static void CheckImportedNobId(Script &, UPNobList const &, NobId const);

	unique_ptr<NNetModel> GetImportedModel();
	wstring const &       GetModelFileName() const { return m_wstrFile2Read; }

	NNetModelWriterInterface & GetImportNMWI() { return * m_upImportedNMWI; }

	/// export ///

	void Export(NNetModelWriterInterface const &);

	int    GetCompactIdVal(NobId const) const;
	size_t NrOfCompactIds()             const;

	NNetModelWriterInterface const & GetExportNMWI() const { return * m_pExportNMWI; }

private:

	vector<unique_ptr<NNetWrapperBase>>  m_wrapVector;

	/// import ///

	unique_ptr<ImportTermination>        m_upTermination;   
	unique_ptr<NNetModelWriterInterface> m_upImportedNMWI;  // valid only during import
	unique_ptr<NNetModel>                m_upImportedModel; // valid only during import
	wstring                              m_wstrFile2Read;

	void importModel();

	friend static unsigned int __stdcall importModelThreadProc(void *);

	/// export ///

	NobIdList                        m_CompactIds;
	NNetModelWriterInterface const * m_pExportNMWI { nullptr };  // valid only during export

	void compress(NNetModelWriterInterface const &);
	void writeHeader(wostream &) const;
};

