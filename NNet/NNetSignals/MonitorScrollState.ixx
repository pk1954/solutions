// MonitorScrollState.ixx
//
// NNetSignals

module;

#include <string>
#include <iostream>
#include <Resource.h>

export module NNetSignals:MonitorScrollState;

import IoUtil;
import IoConstants;
import Script;
import Symtab;
import SaveCast;
import WinManager;
import Commands;
import NNetWrapperHelpers;
import :MonitorWindow;

using std::endl;

export class MonitorScrollState : public Wrapper
{
public:
	using Wrapper::Wrapper;

	void operator() (Script& script) const final
	{
		MonitorWindow             * pMonWin   { static_cast<MonitorWindow*>(WinManager::GetRootWindow(RootWinId(IDM_MONITOR_WINDOW))) };
		PixFpDimension<fMicroSecs>& horzCoord { pMonWin->HorzCoord() };
		PixFpDimension<mV>        & vertCoord { pMonWin->VertCoord() };
		vertCoord.SetPixelSize(ScrReadVoltage   (script), false);
		horzCoord.SetPixelSize(ScrReadfMicroSecs(script), false);
//		horzCoord.SetOffset   (ScrReadfPixel(script), false);
		ScrReadfPixel(script);
//		m_pMonitorWindow->SetHorzScaleLocked(script.ScrReadBool());
		script.ScrReadBool();
		pMonWin->Notify(true);
	}

	void Write(wostream& out) const final
	{
		MonitorWindow             * pMonWin   { static_cast<MonitorWindow*>(WinManager::GetRootWindow(RootWinId(IDM_MONITOR_WINDOW))) };
		PixFpDimension<fMicroSecs>& horzCoord { pMonWin->HorzCoord() };
		PixFpDimension<mV>        & vertCoord { pMonWin->VertCoord() };

		WriteCmdName(out);
		out << vertCoord.GetPixelSize() 
			<< horzCoord.GetPixelSize() 
			<< horzCoord.GetPixelOffset() 
			<< SPACE << pMonWin->IsHorzScaleLocked()
			<< endl;
	}
};
