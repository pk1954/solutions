// NNetModelIO.h 
//
// ModelIO

#pragma once

#include <string>
#include <iostream>
#include <Windows.h>
#include "NNetModelWriterInterface.h"
#include "NobId.h"
#include "UPNobList.h"
#include "InputOutputUI.h"
#include "NNetModel.h"
#include "InputOutputUI.h"
#include "Script.h"

import WrapBase;
import NobIdList;

using std::unique_ptr;
using std::wostream;
using std::wstring;

class NNetModelIO
{
public:
	~NNetModelIO();

	void Initialize();

	/// import ///

	bool Import(wstring const &, unique_ptr<InputOutputUI>);

	static void CheckImportedNobId(Script &, UPNobList const &, NobId const);

	unique_ptr<NNetModel> GetImportedModel();
	wstring const &       GetModelFileName() const { return m_wstrFile2Read; }

	NNetModelWriterInterface & GetImportNMWI() { return * m_upImportedNMWI; }

	/// export ///

	void Export(NNetModelReaderInterface const &, unique_ptr<InputOutputUI>);

	int    GetCompactIdVal(NobId const) const;
	size_t NrOfCompactIds()             const;

	NNetModelReaderInterface const & GetExportNMRI() const { return * m_pExportNMRI; }

private:

	unique_ptr<InputOutputUI>    m_upImportUI;   
	vector<unique_ptr<WrapBase>> m_wrapVector;
	TP_TIMER                   * m_pTpTimer { nullptr };

	/// import ///

	unique_ptr<NNetModelWriterInterface> m_upImportedNMWI;  // valid only during import
	unique_ptr<NNetModel>                m_upImportedModel; // valid only during import
	wstring                              m_wstrFile2Read;

	void importModel();

	friend static unsigned int __stdcall importModelThreadProc(void *);

	/// export ///

	NobIdList                        m_CompactIds;
	NNetModelReaderInterface const * m_pExportNMRI { nullptr };  // valid only during export

	void compress(NNetModelReaderInterface const &);
	void writeHeader(wostream &) const;
};

