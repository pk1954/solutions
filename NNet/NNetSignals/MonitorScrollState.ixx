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
		mV         const voltage          { ScrReadVoltage(script) };
		fMicroSecs const fMsecs           { ScrReadfMicroSecs(script) };
		fPixel     const fPixelOffset     { ScrReadfPixel(script) };
		bool       const bHorzScaleLocked { script.ScrReadBool() };
		MonitorWindow   *pMonWin          { static_cast<MonitorWindow*>(WinManager::GetRootWindow(RootWinId(IDM_MONITOR_WINDOW))) };
		if (pMonWin)
		{
			PixFpDimension<fMicroSecs>& horzCoord { pMonWin->HorzCoord() };
			PixFpDimension<mV>        & vertCoord { pMonWin->VertCoord() };
			vertCoord.SetPixelSize(voltage, false);
			horzCoord.SetPixelSize(fMsecs, false);
			pMonWin->Notify(true);
		}
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
