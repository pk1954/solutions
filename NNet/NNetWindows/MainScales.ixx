// MainScales.ixx
//
// NNetWindows

module;

#include <cassert>
#include <memory>
#include <Windows.h>

export module NNetWin32:MainScales;

import Types;
import Scale;
import Observable;
import Direct2D;
import Uniform2D;
import RootWindow;
import NNetCommands;
import SetGridCmd;
import SetScalesCmd;
import Win32_Util;
import Win32_Util_Resource;
import Resource;

using std::unique_ptr;
using std::make_unique;

export class MainScales
{
public:
	void AppendScaleMenu
	(
		HMENU         hMenu,
		LPCTSTR const title
	) const
	{
		HMENU hMenuPopup = ::PopupMenu(hMenu, title);
		
		::AddMenu(hMenuPopup, MF_STRING, IDM_SCALE_OFF,  L"o&ff");
		::AddMenu(hMenuPopup, MF_STRING, IDM_SCALE_ON,   L"o&n");
		::AddMenu(hMenuPopup, MF_STRING, IDM_SCALE_GRID, L"&grid");
		::Enable(hMenuPopup, IDM_SCALE_OFF, HasScales());
		::Enable(hMenuPopup, IDM_SCALE_ON,  HasGrid() || !HasScales());
		::Enable(hMenuPopup, IDM_SCALE_GRID, !HasGrid());
	}

	void Start
	(
		RootWindow           * pRootWinParent,
		Uniform2D<MicroMeter>& coord,
		Observable           & coordObservable
	)
	{
		m_pRootWinParent = pRootWinParent;

		m_upHorzScale = make_unique<Scale<MicroMeter>>(pRootWinParent->GetWindowHandle(), false, coord.GetXdim());
		m_upVertScale = make_unique<Scale<MicroMeter>>(pRootWinParent->GetWindowHandle(), true,  coord.GetYdim());

		coordObservable.RegisterObserver(*m_upHorzScale.get());
		coordObservable.RegisterObserver(*m_upVertScale.get());

		m_upHorzScale->SetTicksDir(BaseScale::TICKS_DOWN);
		m_upHorzScale->SetAllowUnlock(true);
		m_upHorzScale->SetZoomAllowed(false);

		m_upVertScale->SetTicksDir(BaseScale::TICKS_LEFT);
		m_upVertScale->SetAllowUnlock(true);
		m_upVertScale->SetZoomAllowed(false);
		m_upVertScale->DisplayUnit(false);
	}

	void AdjustScales()
	{
		m_upHorzScale->SetOrthoOffset (m_fPixScaleSize.GetY());
		m_upHorzScale->SetBottomBorder(m_fPixScaleSize.GetY());

		m_upHorzScale->SetLeftBorder (m_fPixScaleSize.GetX());
		m_upVertScale->SetOrthoOffset(m_fPixScaleSize.GetX());

		PixelRectSize const pixRectSize  { m_pRootWinParent->GetClRectSize() };
		PixelPoint    const pixScaleSize { Convert2PixelPoint(m_fPixScaleSize) };
		PIXEL         const pixHeight    { pixRectSize.GetY() - pixScaleSize.GetY() };

		m_upHorzScale->Move(0_PIXEL, pixHeight, pixRectSize .GetX(), pixScaleSize.GetY(), true);
		m_upVertScale->Move(0_PIXEL, 0_PIXEL,   pixScaleSize.GetX(), pixHeight,           true);
		m_upHorzScale->SetUnitOffset(fPixelPoint(5._fPixel - m_fPixScaleSize.GetX(), -3._fPixel));
	}

	void PaintGrid(D2D_driver& graphics)
	{
		if (HasGrid())
		{
			m_upHorzScale->DrawAuxLines(graphics, m_fGridDimFactor);
			m_upVertScale->DrawAuxLines(graphics, m_fGridDimFactor);
		}
	}

	bool HasScales() const { return m_fPixScaleSize.IsNotZero(); }
	bool HasGrid()   const { return m_fGridDimFactor > 0.0f; }

	void SetGrid(bool const bOn, bool const bAnim)
	{
		SetGridCmd::Push
		(
			*m_pRootWinParent, 
			m_fGridDimFactor, // animation parameter
			bOn, 
			bAnim
		);
	}

	void SetScales(bool const bOn, bool const bAnim)
	{
		SetScalesCmd::Push
		(
			*m_pRootWinParent, 
			m_fPixScaleSize,   // animation parameter
			bOn, 
			bAnim
		);
	}

	void HandleCommand(int const iCmd)
	{
		switch (iCmd)
		{
		case IDM_SCALE_OFF:
			SetScales(false, true);
			SetGrid  (false, true);
			break;

		case IDM_SCALE_ON:
			SetScales(true,  true);
			SetGrid  (false, true);
			break;

		case IDM_SCALE_GRID:
			SetScales(true, true);
			SetGrid  (true, true);
			break;

		case IDD_SCALES_UPDATE:
			AdjustScales();
			break;

		case IDD_GRID_UPDATE:
			m_pRootWinParent->Notify(true);
			break;

		default:
			assert(false);
		}
	}

	fPixel VerticalOffset() const
	{
		return HasScales()
			? m_upVertScale->GetOrthoOffset()
			: 0._fPixel;
	}

private:

	RootWindow                  * m_pRootWinParent { nullptr };
	unique_ptr<Scale<MicroMeter>> m_upHorzScale    {};
	unique_ptr<Scale<MicroMeter>> m_upVertScale    {};
	fPixelPoint                   m_fPixScaleSize  { fPP_ZERO };
	float                         m_fGridDimFactor { 0.0f };
};