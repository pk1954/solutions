// Win32_MainWindow.ixx
//
// NNetWindows

module;

#include <Windows.h>
#include "Resource.h"
#include "BaseKnot.h"
#include "UPNobList.h"
#include "MicroMeterPntVector.h"
#include "ConnAnimationCommand.h"
#include "NNetModelCommands.h"
#include "NNetModelReaderInterface.h"
#include "win32_NNetWindow.h"

export module MainWindow;

import ActionTimer;
import PixelTypes;
import Preferences;

using std::unique_ptr;

export class MainWindow : public NNetWindow
{
public:

	void Start
	(
		HWND   const,
		bool   const,
		fPixel const,
		Preferences       &,
		NNetController    &,
		NNetModelCommands &,
		Observable        &,
		Observable        &,
		ActionTimer * const
	);

	void Stop () final;
	void Reset();

	LPARAM AddContextMenuEntries(HMENU const) final;

	NobId GetTargetNobId () const { return m_nobTarget; }

	template <typename T>
	void NNetMove(T const & delta)	
	{ 
		GetDrawContext().Move(delta); 
		Notify(true);  
		if (m_pCoordObservable)
			m_pCoordObservable->NotifyAll(false);
	}

	MicroMeterPnt GetCursorPos() const;

	void CenterModel();
	void CenterSelection();
	void AnimateArrows();
	void SetSensorPoints();
	bool SensorsPointsVisible() const {	return m_bShowPnts; }

	void OnPaint() final;
	bool OnCommand        (WPARAM const, LPARAM const, PixelPoint const) final;
	bool OnRButtonDown    (WPARAM const, LPARAM const) final;
	void OnMouseWheel     (WPARAM const, LPARAM const) final;
	void OnMouseMove      (WPARAM const, LPARAM const) final;
	void OnLButtonDblClick(WPARAM const, LPARAM const) final;
	bool OnLButtonUp      (WPARAM const, LPARAM const) final;
	bool OnRButtonUp      (WPARAM const, LPARAM const) final;
	bool OnSize           (PIXEL  const, PIXEL  const) final;

private:

	inline static MicroMeter const STD_ARROW_SIZE { 30.0_MicroMeter };

	MicroMeter          m_arrowSize            { STD_ARROW_SIZE };
	Preferences       * m_pPreferences         { nullptr };
	ActionTimer       * m_pDisplayTimer        { nullptr };
	MicroMeterRect      m_rectSelection        { };
	MicroMeterPnt       m_umPntSelectionAnchor { };
	NobId               m_nobTarget            { NO_NOB };
	Observable        * m_pCoordObservable     { nullptr };
	Observable        * m_pCursorPosObservable { nullptr };
	NNetModelCommands * m_pModelCommands       { nullptr };
	bool                m_bShowPnts            { false };
	ConnectionType      m_connType             { ConnectionType::ct_none };

	void setNoTarget      ();
	void setTargetNob     ();
	void setHighlightedNob(MicroMeterPnt const &);
	void centerAndZoomRect(UPNobList::SelMode const, float const);

	void DoPaint() final;

	bool UserProc(UINT const, WPARAM const, LPARAM const) final;
};
