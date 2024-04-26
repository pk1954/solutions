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
import NNetPreferences;
import :Compute;

using std::wostream;
using std::wstring;
using std::wcout;

export class NNetController
{
public:
	NNetController() = default;

	void Initialize(Compute *const, SlowMotionRatio *const);

	virtual ~NNetController();

	void SetModelInterface(NNetModelReaderInterface * const);
	bool HandleCommand(int const, LPARAM const, MicroMeterPnt const = NP_NULL);

	static wstring AskModelFile(enum class tFileMode const);

private:

	bool       IsTraceOn  () const { return m_bTrace; }
	wostream & TraceStream()       { return wcout; }

	bool processUIcommand   (int const, LPARAM const = 0, MicroMeterPnt const = NP_NULL);
	bool processModelCommand(int const, LPARAM const = 0, MicroMeterPnt const = NP_NULL);
	void deleteSignal       ();

	bool                       m_bTrace           { true };
	HCURSOR                    m_hCrsrWait        { nullptr };
	Compute                  * m_pCompute         { nullptr };
	NNetModelReaderInterface * m_pNMRI            { nullptr };
	SlowMotionRatio          * m_pSlowMotionRatio { nullptr };
};				          
