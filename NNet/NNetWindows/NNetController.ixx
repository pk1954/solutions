// NNetController.ixx
//
// NNetWindows

module;

#include <compare>
#include <iostream>
#include <string>
#include <iostream>
#include <Windows.h>

export module NNetWin32:NNetController;

import Observable;
import SlowMotionRatio;
import Types;
import WinManager;
import CommandStack;
import NNetModel;
import NNetSignals;
import :NNetCommandHandler;
import :Compute;
import :AppTitle;

using std::wostream;
using std::wstring;
using std::wcout;

export class NNetController: public NNetCommandHandler
{
public:
	NNetController() = default;

	void Initialize(Compute *const, SlowMotionRatio *const, AppTitle *const);

	virtual ~NNetController();

	void SetModelInterface(NNetModelReaderInterface const * const);
	bool HandleCommand(int const, LPARAM const, MicroMeterPnt const = NP_NULL) final;

	bool AskAndSave();

private:

	bool SaveModelAs();
	bool SaveModel();
	bool SaveScanAs();
	void WriteModel(wstring const&);

	bool       IsTraceOn  () const { return m_bTrace; }
	wostream & TraceStream()       { return wcout; }

	bool processUIcommand   (int const, LPARAM const = 0, MicroMeterPnt const = NP_NULL);
	bool processModelCommand(int const, LPARAM const = 0, MicroMeterPnt const = NP_NULL);

	bool                            m_bTrace           { true };
	HCURSOR                         m_hCrsrWait        { nullptr };
	HCURSOR                         m_hCrsrArrow       { nullptr };
	Compute                       * m_pCompute         { nullptr };
	NNetModelReaderInterface const* m_pNMRI            { nullptr };
	AppTitle                      * m_pAppTitle        { nullptr };
	SlowMotionRatio               * m_pSlowMotionRatio { nullptr };
};				          
