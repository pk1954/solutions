// NNetModelIO.ixx
//
// ModelIO

module;

#include <vector>
#include <string>
#include <iostream>
#include <Windows.h>

export module NNetModelIO;

import Observable;
import Types;
import Wrapper;
import Script;
import InputOutputUI;
import NNetModel;
import ThreadPoolTimer;

using std::unique_ptr;
using std::make_unique;
using std::vector;

export class NNetModelIO
{
public:

	static void Initialize(Observable * const);

	/// import ///

	static bool Import(wstring const, unique_ptr<InputOutputUI>);

	static void CheckImportedNobId(Script&, UPNobList const&, NobId const);

	static NNetModelWriterInterface& GetImportNMWI   () { return *m_upImportedNMWI; }
	static unique_ptr<Model>         GetImportedModel() { return move(m_upImportedModel); }
	static wstring const&            GetModelFileName() { return m_wstrFile2Read; }
	static void SetModelFileName(wstring const& name)   { m_wstrFile2Read = name; }

	/// export ///

	static void Export(NNetModelReaderInterface const&, unique_ptr<InputOutputUI>);

	static short  GetCompactIdVal(NobId const);
	static size_t NrOfCompactIds();

	static NNetModelReaderInterface const& GetExportNMRI() { return *m_pExportNMRI; }

	template <Wrap_t WRAPPER>
	static WRAPPER * AddModelWrapper(wstring const& name)
	{
		unique_ptr upWrapper { make_unique<WRAPPER>(name) };
		WRAPPER  * pWrapper  { upWrapper.get() };
		m_wrapVector.push_back(move(upWrapper));
		return pWrapper;
	}

private:

	inline static Observable * m_pLockModelObservable { nullptr };

	inline static unique_ptr<InputOutputUI>   m_upImportUI;
	inline static vector<unique_ptr<Wrapper>> m_wrapVector;
	inline static ThreadPoolTimer             m_timer;

	/// import ///

	inline static unique_ptr<NNetModelWriterInterface> m_upImportedNMWI;  // valid only during import
	inline static unique_ptr<Model>                    m_upImportedModel; // valid only during import
	inline static wstring                              m_wstrFile2Read;
	inline static Radian                               m_radDislocate;

	static void importModel();
	static void fixProblems();
	static void dislocate(PosNob &);

	friend static unsigned int __stdcall importModelThreadProc(void*);

	/// export ///

	inline static size_t                           m_nrOfcompactIds { 0 };
	inline static NobIdList                        m_CompactIds;
	inline static NNetModelReaderInterface const * m_pExportNMRI { nullptr };  // valid only during export

	static void compress(NNetModelReaderInterface const&);
	static void writeHeader(wostream&);
};
