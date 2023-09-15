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
import SoundInterface;
import SlowMotionRatio;
import Types;
import WinManager;
import CommandStack;
import NNetModel;
import NNetSignals;
import NNetPreferences;
import :ComputeThread;

using std::wostream;
using std::wcout;

export class NNetController
{
public:
	NNetController() = default;

	void Initialize
	(
		ComputeThread   * const,
		SlowMotionRatio * const,
		Sound           * const,
		CommandStack    * const,
		MonitorWindow   * const,
		Observable      * const
	);

	virtual ~NNetController();

	void SetModelInterface(NNetModelReaderInterface * const);
	bool HandleCommand(int const, LPARAM const, MicroMeterPnt const = NP_NULL);

private:

	bool       IsTraceOn  () const { return m_bTrace; }
	wostream & TraceStream()       { return wcout; }

	bool processUIcommand   (int const, LPARAM const);
	bool processModelCommand(int const, LPARAM const = 0, MicroMeterPnt const = NP_NULL);
	void deleteSignal       (NNetSignal const *);

	bool                       m_bTrace                 { true };
	bool                       m_bBlockedUI             { false };
	HCURSOR                    m_hCrsrWait              { nullptr };
	Sound                    * m_pSound                 { nullptr };
	ComputeThread            * m_pComputeThread         { nullptr };
	NNetModelReaderInterface * m_pNMRI                  { nullptr };
	SlowMotionRatio          * m_pSlowMotionRatio       { nullptr };
	CommandStack             * m_pCommandStack          { nullptr };
	MonitorWindow            * m_pMonitorWindow         { nullptr };
	Observable               * m_pStaticModelObservable { nullptr };
};				          
