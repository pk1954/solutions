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
import RunTime;
import SaveCast;
import Commands;
import NNetWrapperHelpers;
import :MonitorWindow;

using std::endl;

export class MonitorScrollState : public Wrapper
{
public:

	MonitorScrollState
	(
		wstring const & wstrName,
		MonitorWindow * pMonitorWindow = nullptr
	)
      : Wrapper(wstrName),
 		m_pMonitorWindow(pMonitorWindow)
	{}

	void operator() (Script& script) const final
	{
		mV         const voltage          { ScrReadVoltage(script) };
		fMicroSecs const fMsecs           { ScrReadfMicroSecs(script) };
		fPixel     const fPixelOffset     { ScrReadfPixel(script) };
		bool       const bHorzScaleLocked { script.ScrReadBool() };
		if (m_pMonitorWindow)
		{
			PixFpDimension<fMicroSecs>& horzCoord { m_pMonitorWindow->HorzCoord() };
			PixFpDimension<mV>        & vertCoord { m_pMonitorWindow->VertCoord() };
			vertCoord.SetPixelSize(voltage, false);
			horzCoord.SetPixelSize(fMsecs, false);
			m_pMonitorWindow->Notify(true);
		}
	}

	void Write(wostream& out) const final
	{
		if (m_pMonitorWindow)
		{
			PixFpDimension<fMicroSecs>& horzCoord { m_pMonitorWindow->HorzCoord() };
			PixFpDimension<mV>        & vertCoord { m_pMonitorWindow->VertCoord() };

			WriteCmdName(out);
			out << vertCoord.GetPixelSize() 
				<< horzCoord.GetPixelSize() 
				<< horzCoord.GetPixelOffset() 
				<< SPACE << m_pMonitorWindow->IsHorzScaleLocked()
				<< endl;
		}
	}

private:

	MonitorWindow * m_pMonitorWindow { nullptr };
};
