// MainWindow.ixx
//
// NNetWindows

module;

#include "Resource.h"

export module NNetWin32:MainWindow;

import std;
import Types;
import HiResTimer;
import Vector2D;
import ColorLUT;
import D2D_ColorLUT;
import NNetModel;
import NNetSignals;
import NNetCommands;
import WinBasics;
import :NNetCommandHandler;
import :SelectionMenu;
import :NNetWindow;
import :MainScales;
import :Compute;

using std::unique_ptr;
using std::optional;
using std::nullopt;

export class MainWindow : public NNetWindow
{
public:

	void Start
	(
		HWND const,
		bool const,
		NNetCommandHandler&,
		Observable&,
		Observable&,
		Observable&,
		Compute const&,  
		HiResTimer    * const,
		MonitorWindow * const
	);

	void Reset();

	LPARAM AddContextMenuEntries(HMENU const) final;

	template <typename T>
	void NNetMove(T const& delta)
	{
		GetDrawContext().Move(delta);
		if (m_pCoordObservable)
			m_pCoordObservable->NotifyAll(true);   // update immediately
	}

	MicroMeterPnt GetCursorPos() const;

	NobId GetHighlightedNobId()         const { return m_nobIdHighlighted; }
	bool  IsHighlighted(NobId const id) const { return id == m_nobIdHighlighted; }
	bool  IsHighlighted(Nob const& nob) const { return IsHighlighted(nob.GetId()); }

	bool HasScales() const final { return m_mainScales.HasScales(); }
	bool HasGrid  () const final { return m_mainScales.HasGrid(); }

	void SetGrid  (bool const bOn, bool const bAnim) final { m_mainScales.SetGrid  (bOn, bAnim); }
	void SetScales(bool const bOn, bool const bAnim) final { m_mainScales.SetScales(bOn, bAnim); }

	void CenterModel();
	void CenterSelection();

	void OnPaint()                                                       final;
	void OnChar           (WPARAM const, LPARAM const)                   final;
	bool OnCommand        (WPARAM const, LPARAM const, PixelPoint const) final;
	bool OnRButtonDown    (WPARAM const, LPARAM const)                   final;
	bool OnMouseWheel     (WPARAM const, LPARAM const)                   final;
	void OnMouseMove      (WPARAM const, LPARAM const)                   final;
	void OnLButtonDblClick(WPARAM const, LPARAM const)                   final;
	bool OnLButtonUp      (WPARAM const, LPARAM const)                   final;
	bool OnLButtonDown    (WPARAM const, LPARAM const)                   final;
	bool OnSize           (PIXEL  const, PIXEL  const)                   final;
	bool OnMove           (PIXEL  const, PIXEL  const)                   final;

private:

	MicroMeterPnt            m_umDelta                { NP_ZERO };
	MicroMeter               m_umArrowSize            { 0._MicroMeter };
	Compute           const *m_pCompute               { nullptr }; 
	HiResTimer              *m_pDisplayTimer          { nullptr };
	Observable              *m_pCoordObservable       { nullptr };
	Observable              *m_pCursorPosObservable   { nullptr };
	Observable              *m_pStaticModelObservable { nullptr };
	NobId                    m_nobIdHighlighted       { NO_NOB };
	NobId                    m_nobIdTarget            { NO_NOB };
	SigGenId                 m_idSigGenUnderCrsr      { NO_SIGGEN };
	SensorId                 m_sensorIdSelected       { SensorId::NULL_VAL() };
	optional<CardPoint>      m_scanAreaHandleSelected { nullopt };	
	unique_ptr<D2D_ColorLUT> m_upScanLut;
	SelectionMenu            m_selectionMenu;
	MainScales               m_mainScales;

	bool     setHighlightedNob   (MicroMeterPnt const&);
	bool     setHighlightedSensor(MicroMeterPnt const&);
	bool     selectSignalHandle  (MicroMeterPnt const&);
	bool     setScanAreaHandle   (MicroMeterPnt const&);
	void     centerAndZoomRect(UPNobList::SelMode const, float const);
	bool     connectionAllowed();
	void     select(NobId const);
	void     drawModel(DrawContext const&);
	SigGenId getSigGenId(LPARAM const);
	SigGenId getSigGenId(fPixelPoint const &);
	bool     selectSigGen(SigGenId const);
	void	 drawScanArea();
	void     drawInputCable(InputLine const &) const;
	void     connect(NobId const, NobId const);
	bool     selectionCommand(WPARAM const);

	void     PaintGraphics() final;
	bool     UserProc(UINT const, WPARAM const, LPARAM const) final;
};
