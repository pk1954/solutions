// Win32_MainWindow.ixx
//
// NNetWindows

module;

#include <Windows.h>
#include <memory>
#include "Resource.h"

export module MainWindow;

import ActionTimer;
import Types;
import Preferences;
import SelectionMenu;
import NNetWindow;
import NNetModelCommands;
import NNetModel;

using std::unique_ptr;

export class MainWindow : public NNetWindow
{
public:

	void Start
	(
		HWND   const,
		bool   const,
		fPixel const,
		Preferences&,
		NNetController&,
		NNetModelCommands&,
		Observable&,
		Observable&,
		ActionTimer* const
	);

	void Stop() final;
	void Reset();

	LPARAM AddContextMenuEntries(HMENU const) final;

	template <typename T>
	void NNetMove(T const& delta)
	{
		GetDrawContext().Move(delta);
		Notify(true);
		if (m_pCoordObservable)
			m_pCoordObservable->NotifyAll(false);
	}

	MicroMeterPnt GetCursorPos() const;

	NobId GetHighlightedNobId() const { return m_nobIdHighlighted; }

	void CenterModel();
	void CenterSelection();
	void AnimateArrows();
	void SetSensorPoints();
	bool SensorsPointsVisible() const { return m_bShowPnts; }

	void OnPaint()                                                       final;
	void OnChar           (WPARAM const, LPARAM const)                   final;
	bool OnCommand        (WPARAM const, LPARAM const, PixelPoint const) final;
	bool OnRButtonDown    (WPARAM const, LPARAM const)                   final;
	void OnMouseWheel     (WPARAM const, LPARAM const)                   final;
	void OnMouseMove      (WPARAM const, LPARAM const)                   final;
	void OnLButtonDblClick(WPARAM const, LPARAM const)                   final;
	bool OnLButtonUp      (WPARAM const, LPARAM const)                   final;
	bool OnSize           (PIXEL  const, PIXEL  const)                   final;

private:

	inline static MicroMeter const STD_ARROW_SIZE { 30.0_MicroMeter };

	MicroMeter         m_arrowSize            { STD_ARROW_SIZE };
	Preferences*       m_pPreferences         { nullptr };
	ActionTimer*       m_pDisplayTimer        { nullptr };
	MicroMeterPnt      m_umPntSelectionAnchor { };
	Observable*        m_pCoordObservable     { nullptr };
	Observable*        m_pCursorPosObservable { nullptr };
	NNetModelCommands* m_pModelCommands       { nullptr };
	bool               m_bShowPnts            { false };
	NobId              m_nobIdHighlighted     { NO_NOB };
	NobId              m_nobIdTarget          { NO_NOB };
	SelectionMenu      m_SelectionMenu;

	NobId findTargetNob(MicroMeterPnt const&);
	bool  setHighlightedNob   (MicroMeterPnt const&);
	bool  setHighlightedSensor(MicroMeterPnt const&);
	void  centerAndZoomRect(UPNobList::SelMode const, float const);
	bool  connectionAllowed();
	void  select(NobId const);

	void  DoPaint() final;

	bool  UserProc(UINT const, WPARAM const, LPARAM const) final;
};
