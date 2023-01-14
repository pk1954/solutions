// NNetModelIO.ixx
//
// ModelIO

module;

#include <vector>
#include <string>
#include <iostream>
#include <Windows.h>

export module NNetModelIO;

import WrapBase;
import Script;
import InputOutputUI;
import NNetModel;

using std::unique_ptr;
using std::wostream;
using std::wstring;
using std::vector;

export class NNetModelIO
{
public:
	~NNetModelIO();

	void Initialize();

	/// import ///

	bool Import(wstring const&, unique_ptr<InputOutputUI>);

	static void CheckImportedNobId(Script&, UPNobList const&, NobId const);

	unique_ptr<Model> GetImportedModel();
	wstring const& GetModelFileName() const { return m_wstrFile2Read; }

	NNetModelWriterInterface& GetImportNMWI() { return *m_upImportedNMWI; }

	/// export ///

	void Export(NNetModelReaderInterface const&, unique_ptr<InputOutputUI>);

	int    GetCompactIdVal(NobId const) const;
	size_t NrOfCompactIds()             const;

	NNetModelReaderInterface const& GetExportNMRI() const { return *m_pExportNMRI; }

private:

	unique_ptr<InputOutputUI>    m_upImportUI;
	vector<unique_ptr<WrapBase>> m_wrapVector;
	TP_TIMER* m_pTpTimer{ nullptr };

	/// import ///

	unique_ptr<NNetModelWriterInterface> m_upImportedNMWI;  // valid only during import
	unique_ptr<Model>                    m_upImportedModel; // valid only during import
	wstring                              m_wstrFile2Read;
	Radian                               m_radDislocate;

	void importModel();
	void fixProblems();
	void dislocate(PosNob &);

	friend static unsigned int __stdcall importModelThreadProc(void*);

	/// export ///

	size_t                           m_nrOfcompactIds { 0 };
	NobIdList                        m_CompactIds;
	NNetModelReaderInterface const * m_pExportNMRI{ nullptr };  // valid only during export

	void compress(NNetModelReaderInterface const&);
	void writeHeader(wostream&) const;
};

