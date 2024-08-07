// ColLutDesigner.cpp
//
// Toolbox\win32_utilities

module;

#include <Windows.h> 

module ColLutDesigner;

import WinBasics;
import Win32_Util_Resource;
import ColorDialog;

static const float F_MAX_COL_INDEX { 255.0f };

fPixel ColLutDesigner::colIndex2pos(ColIndex const colIndex) const
{ 
    return (GetClientWidth() / F_MAX_COL_INDEX) * Cast2Float(colIndex); 
}

ColIndex ColLutDesigner::pos2ColIndex(fPixel const fPix) const
{
    int iRes { Cast2Int(fPix.GetValue() * F_MAX_COL_INDEX / GetClientWidth()) };
    ClipToMinMax(iRes, 0, ColorLUT::MAX_INDEX);
    return ColIndex(iRes);
}

fPixel ColLutDesigner::xPos(BasePointNr const bpNr) const
{ 
    return colIndex2pos(m_pLut->GetColIndex(bpNr)); 
}

fPixelPoint ColLutDesigner::handlePos(BasePointNr const bpNr) const 
{ 
    return fPixelPoint(xPos(bpNr), GetClientHeight());
};

void ColLutDesigner::Start
(
    HWND       const hwndParent,
    ColorLUT * const pLut
) 
{ 
    m_pLut = pLut;
	GraphicsWindow::Initialize
    (
        hwndParent, 
		L"ClassColLutDesWindow",
		WS_POPUPWINDOW|WS_CLIPSIBLINGS|WS_CLIPCHILDREN|WS_CAPTION|WS_SIZEBOX|WS_VISIBLE
    );
    m_pLut->RegisterObserver(*this);
}

LPARAM ColLutDesigner::AddContextMenuEntries(HMENU const hPopupMenu)
{
	AppendMenu(hPopupMenu, MF_STRING, IDM_ADD_SUPPORT_PNT, L"Add support point");
    if (IsBasePointDefined(m_bpSelected))
    {
	    AppendMenu(hPopupMenu, MF_STRING, IDM_EDIT_SUPPORT_PNT, L"Edit support point");
        if (m_pLut->IsDeleteable(m_bpSelected))
	        AppendMenu(hPopupMenu, MF_STRING, IDM_REMOVE_SUPPORT_PNT, L"Remove support point");
    }
	
	// do not call GraphicsWindow::AddContextMenuEntries(hPopupMenu);
    // background color selection doesn't make sense here

	return m_bpSelected; // will be forwarded to HandleContextMenuCommand
}

void ColLutDesigner::PaintGraphics()
{
    if (m_pLut)
    {
        m_pLut->Apply2AllRanges
        (
            [this](BasePointNr const bpStart, BasePointNr const bpEnd)
            {
                fPixelRect fPixRect(xPos(bpStart), 0.0_fPixel, xPos(bpEnd), GetClientHeight());
                m_upGraphics->FillGradientRect(fPixRect, m_pLut->GetColor(bpStart), m_pLut->GetColor(bpEnd));
            }
        );
        m_pLut->Apply2AllBasePoints
        (
            [this](BasePointNr const bpNr)
            {
                fPixel const DIAMOND_SIZE{ 8.0_fPixel };
                fPixelPoint const fPixPos{ handlePos(bpNr) };
                m_upGraphics->FillDiamond
                (
                    fPixPos,
                    DIAMOND_SIZE + ((bpNr == m_bpSelected) ? 4.0_fPixel : 1.0_fPixel),
                    D2D1::ColorF::White
                );
                m_upGraphics->FillDiamond(fPixPos, DIAMOND_SIZE, m_pLut->GetColor(bpNr)); // D2D1::ColorF::Black);
            }
        );
    }
}

void ColLutDesigner::OnMouseMove(WPARAM const wParam, LPARAM const lParam)
{
    if (m_pLut)
    {
	    fPixelPoint const fPixCrsrPos { GetCrsrPosFromLparamF(lParam) };
	    if (wParam & MK_LBUTTON)
	    {
            if (m_pLut->IsMoveable(m_bpSelected))
            {
                if (ColIndex const i = pos2ColIndex(fPixCrsrPos.GetX()); ColorLUT::IsBorderIndex(i))
                    removeSupportPoint(m_bpSelected);
                else
                    moveSupportPoint(m_bpSelected, i);
            }
	    }
    	else  // left button not pressed: select
	    { 
    	    fPixel fPixDistBest { 10000._fPixel };
            m_bpSelected = NoBasePoint;
            m_pLut->Apply2AllBasePoints
            (
                [this, &fPixCrsrPos, &fPixDistBest](BasePointNr const bpNr)
                {
            	    fPixel const fPixCandidate = Distance(fPixCrsrPos, handlePos(bpNr));
	                if ((fPixCandidate <= 20.0_fPixel) && (fPixCandidate < fPixDistBest))
	                {
		                fPixDistBest = fPixCandidate;
		                m_bpSelected = bpNr;
	                }
                }
            );
		    Trigger();   // cause repaint
        }
	}
	GraphicsWindow::OnMouseMove(wParam, lParam);
}

void ColLutDesigner::OnLButtonDblClick(WPARAM const wParam, LPARAM const lParam)
{
    if (IsBasePointDefined(m_bpSelected))
        editSupportPoint(m_bpSelected);
    else
        addSupportPoint(GetCrsrPosFromLparamF(lParam));
}

void ColLutDesigner::editSupportPoint(BasePointNr const bpNr)
{
	COLORREF colorRef { Convert2COLORREF(m_pLut->GetColor(bpNr)) };
    if (ColorDialog(GetWindowHandle(), colorRef))
    {
        m_pLut->SetColor(bpNr, Color(colorRef));
        m_pLut->Construct();
    }
}

void ColLutDesigner::removeSupportPoint(BasePointNr const bpNr)
{
    if (IsBasePointDefined(bpNr))
    {
        m_pLut->RemoveBasePoint(bpNr);
        m_bpSelected = NoBasePoint;
        m_pLut->Construct();
    }
}

void ColLutDesigner::moveSupportPoint(BasePointNr const bpNr, ColIndex const colIndexNew)
{
    m_pLut->SetColIndex(m_bpSelected, colIndexNew);
    m_pLut->Construct();
}

BasePointNr ColLutDesigner::addSupportPoint(fPixelPoint const& fPixPos)
{
    ColIndex    const colIndex { pos2ColIndex(fPixPos.GetX()) };
    BasePointNr const bpNrNew  { m_pLut->AddBasePoint(colIndex) };
    m_pLut->Construct();
    return bpNrNew;
}

bool ColLutDesigner::OnCommand(WPARAM const wParam, LPARAM const lParam, PixelPoint const pixPoint)
{
    BasePointNr const bpNr { static_cast<BasePointNr>(lParam) };
	bool bRes = false;

	switch (int const wmId = LOWORD(wParam))
	{
	case IDM_ADD_SUPPORT_PNT:
        addSupportPoint(Convert2fPixelPoint(pixPoint));
		break;

	case IDM_EDIT_SUPPORT_PNT:
        editSupportPoint(bpNr);
		break;

	case IDM_REMOVE_SUPPORT_PNT:
        removeSupportPoint(bpNr);
		break;

    default:
	    bRes = GraphicsWindow::OnCommand(wParam, lParam, pixPoint);
		break;
	}

	return bRes; 
}

bool ColLutDesigner::OnLButtonDown(WPARAM const wParam, LPARAM const lParam)
{
	SetCapture();
	return false;
}

bool ColLutDesigner::OnLButtonUp(WPARAM const wParam, LPARAM const lParam)
{
	ReleaseCapture();
	return false;
}

void ColLutDesigner::OnMouseLeave()
{
	m_bpSelected = NoBasePoint;
	Trigger();   // cause repaint
	GraphicsWindow::OnMouseLeave();
}
