// MonitorScrollState.ixx
//
// NNetSignals

module;

#include <string>
#include <iostream>

export module NNetSignals:MonitorScrollState;

import IoUtil;
import IoConstants;
import Script;
import Symtab;
import SaveCast;
import NNetModelIO;
import Wrapper;
import NNetWrapperHelpers;
import :MonitorWindow;

using std::endl;

export class MonitorScrollState : public Wrapper
{
public:
	using Wrapper::Wrapper;

	void SetMonitorWindow(MonitorWindow* const pMonitorWindow)
	{
		m_pMonitorWindow = pMonitorWindow;
	}

	void operator() (Script& script) const final
	{
		PixFpDimension<fMicroSecs>& horzCoord { m_pMonitorWindow->HorzCoord() };
		PixFpDimension<mV>        & vertCoord { m_pMonitorWindow->VertCoord() };
		vertCoord.SetPixelSize(ScrReadVoltage(script), false);
		horzCoord.SetPixelSize(ScrReadfMicroSecs(script), false);
		horzCoord.SetOffset   (ScrReadfPixel(script), false);
		m_pMonitorWindow->SetHorzScaleLocked(script.ScrReadBool());
		m_pMonitorWindow->Notify(true);
	}

	void Write(wostream& out) const final
	{
		PixFpDimension<fMicroSecs>& horzCoord { m_pMonitorWindow->HorzCoord() };
		PixFpDimension<mV>& vertCoord { m_pMonitorWindow->VertCoord() };

		WriteCmdName(out);
		out << vertCoord.GetPixelSize() 
			<< horzCoord.GetPixelSize() 
			<< horzCoord.GetPixelOffset() 
			<< SPACE << m_pMonitorWindow->IsHorzScaleLocked()
			<< endl;
	}

private:

	MonitorWindow* m_pMonitorWindow { nullptr };
};
