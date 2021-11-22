// win32_MainWindow.h : 
//
// NNetWindows

#pragma once

#include "Resource.h"
#include "win32_scale.h"
#include "BaseKnot.h"
#include "MicroMeterPntVector.h"
#include "NNetModelReaderInterface.h"
#include "win32_scale.h"
#include "win32_animation.h"
#include "win32_NNetWindow.h"

class WinCommands;
class NNetModelCommands;
class ConnAnimationCommand;

using std::unique_ptr;

class MainWindow : public NNetWindow
{
public:

	void Start
	(
		HWND                     const,
		DWORD                    const,
		bool                     const,
		fPixel                   const,
		NNetModelReaderInterface const &,
		NNetController                 &,
		NNetModelCommands              &,
		Observable                     &,
		Observable                     &
	);

	void Stop();
	void Reset();

	virtual LPARAM AddContextMenuEntries(HMENU const);

	NobId GetTargetNobId     () const { return m_nobTarget; }
	NobId GetHighlightedNobId() const { return m_nobHighlighted; }
	bool  AnyNobsSelected    () const { return m_pNMRI->AnyNobsSelected(); }

	template <typename T>
	void NNetMove(T const & delta)	
	{ 
		GetDrawContext().Move(delta); 
		Notify(true);  
		if (m_pCoordObservable)
			m_pCoordObservable->NotifyAll(false);
	}

	MicroMeterPnt const GetCursorPos() const;

	void CenterModel();
	void CenterSelection();
	void ShowArrows(bool const);
	bool ArrowsVisible() const; 

	bool OnCommand           (WPARAM const, LPARAM const, PixelPoint const) final;
	bool OnSize              (WPARAM const, LPARAM const) final;
	bool OnRButtonDown       (WPARAM const, LPARAM const) final;
	void OnMouseWheel        (WPARAM const, LPARAM const) final;
	void OnMouseMove         (WPARAM const, LPARAM const) final;
	void OnLeftButtonDblClick(WPARAM const, LPARAM const) final;
	void OnLButtonUp         (WPARAM const, LPARAM const) final;
	bool OnRButtonUp         (WPARAM const, LPARAM const) final;
	void OnPaint             () final;

private:
	 
	inline static MicroMeter const STD_ARROW_SIZE { 30.0_MicroMeter };
	
//	Scale<fMicroSecs> m_horzScale;

	MicroMeter m_arrowSize { STD_ARROW_SIZE };

	MicroMeterRect      m_rectSelection        { };
	MicroMeterPnt       m_umPntSelectionAnchor { };
	NobId               m_nobHighlighted       { NO_NOB };
	NobId               m_nobTarget            { NO_NOB };
	bool                m_bTargetFits          { false };
	Observable        * m_pCoordObservable     { nullptr };
	Observable        * m_pCursorPosObservable { nullptr };
	NNetModelCommands * m_pModelCommands       { nullptr };

	void setNoTarget      ();
	void setTargetNob     ();
	void setHighlightedNob(MicroMeterPnt const &);
	void centerAndZoomRect(UPNobList::SelMode const, float const);
	void zoomStep         (float const, fPixelPoint const);

	virtual void doPaint();

	virtual bool UserProc(UINT const, WPARAM const, LPARAM const);
};
