// ColLutDesigner.cpp
//
// Toolbox\win32_utilities

module;

#include <bit> 
#include <Windows.h> 

module ColLutDesigner;

import Win32_Util_Resource;
import ColorDialog;

using std::bit_cast;

static const float F_MAX_COL_INDEX { 255.0f };

fPixel ColLutDesigner::colIndex2pos(ColIndex const colIndex) const
{ 
    return (GetClientWidth() / F_MAX_COL_INDEX) * Cast2Float(colIndex); 
}

ColIndex ColLutDesigner::pos2ColIndex(fPixel const fPix) const
{
    int iRes { Cast2Int(fPix.GetValue() * F_MAX_COL_INDEX / GetClientWidth()) };
    if (iRes < 0)
        iRes = 0;
    if (iRes > ColorLUT::MAX_INDEX)
        iRes = ColorLUT::MAX_INDEX;
    return ColIndex(iRes);
}

fPixel ColLutDesigner::xPos(BP const& bp) const
{ 
    return colIndex2pos(bp.colIndex); 
}

fPixelPoint ColLutDesigner::handlePos(BP const& bp) const 
{ 
    return fPixelPoint(xPos(bp), GetClientHeight());
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
}

LPARAM ColLutDesigner::AddContextMenuEntries(HMENU const hPopupMenu)
{
	AppendMenu(hPopupMenu, MF_STRING, IDM_ADD_SUPPORT_PNT,  L"Add support point");
    if (m_pBpSelected)
    {
	    AppendMenu(hPopupMenu, MF_STRING, IDM_EDIT_SUPPORT_PNT,   L"Edit support point");
	    AppendMenu(hPopupMenu, MF_STRING, IDM_REMOVE_SUPPORT_PNT, L"Remove support point");
    }
	
	// do not call GraphicsWindow::AddContextMenuEntries(hPopupMenu);
    // background color selection doesn't make sense

	return bit_cast<LPARAM>(m_pBpSelected); // will be forwarded to HandleContextMenuCommand
}

void ColLutDesigner::PaintGraphics()
{
    if (m_pLut)
    {
        m_pLut->Apply2AllRanges
        (
            [this](BP const& bpStart, BP const& bpEnd)
            {
                fPixelRect fPixRect(xPos(bpStart), 0.0_fPixel, xPos(bpEnd), GetClientHeight());
                m_upGraphics->FillGradientRect(fPixRect, bpStart.col, bpEnd.col);
            }
        );
        m_pLut->Apply2AllBasePoints
        (
            [this](BP const& bp)
            {
                fPixel const DIAMOND_SIZE { 8.0_fPixel };
                fPixelPoint const fPixPos { handlePos(bp) };
                m_upGraphics->FillDiamond
                (
                    fPixPos, 
                    DIAMOND_SIZE + ((&bp == m_pBpSelected) ? 4.0_fPixel : 1.0_fPixel), 
                    D2D1::ColorF::White
                );
                m_upGraphics->FillDiamond(fPixPos, DIAMOND_SIZE, D2D1::ColorF::Black);
            }
        );
    }
}

void ColLutDesigner::OnMouseMove(WPARAM const wParam, LPARAM const lParam)
{
    if (m_pLut)
    {
	    PixelPoint  const pixCrsrPos  { GetCrsrPosFromLparam(lParam) };
	    fPixelPoint const fPixCrsrPos { Convert2fPixelPoint(pixCrsrPos) };

	    if (wParam & MK_LBUTTON)
	    {
            if (m_pBpSelected)
            {
                if (m_pLut->IsMoveable(m_pBpSelected))
                {
                    m_pBpSelected->colIndex = pos2ColIndex(fPixCrsrPos.GetX());
                    m_pLut->Construct();
		            Trigger();   // cause repaint
                }
            }
	    }
    	else  // left button not pressed: select
	    { 
    	    fPixel fPixDistBest { 10000._fPixel };
            m_pBpSelected = nullptr;
            m_pLut->Apply2AllBasePoints
            (
                [this, &fPixCrsrPos, &fPixDistBest](ColorLUT::BasePoint & bp)
                {
            	    fPixel const fPixCandidate = Distance(fPixCrsrPos, handlePos(bp));
	                if ((fPixCandidate <= 20.0_fPixel) && (fPixCandidate < fPixDistBest))
	                {
		                fPixDistBest  = fPixCandidate;
		                m_pBpSelected = &bp;
	                }
                }
            );
		    Trigger();   // cause repaint
        }
	}
	GraphicsWindow::OnMouseMove(wParam, lParam);
}

bool ColLutDesigner::OnCommand(WPARAM const wParam, LPARAM const lParam, PixelPoint const pixPoint)
{
	bool bRes = false;

	switch (int const wmId = LOWORD(wParam))
	{
	case IDM_ADD_SUPPORT_PNT:
        {
    	    fPixelPoint const fPixCrsrPos { Convert2fPixelPoint(pixPoint) };
            ColIndex    const colIndex    { pos2ColIndex(fPixCrsrPos.GetX()) };
            m_pLut->AddBasePoint(colIndex);
            m_pLut->Construct();
		    Trigger();   // cause repaint
        }
		break;

	case IDM_EDIT_SUPPORT_PNT:
        {
    	    fPixelPoint const fPixCrsrPos { Convert2fPixelPoint(pixPoint) };
            ColIndex    const colIndex    { pos2ColIndex(fPixCrsrPos.GetX()) };
	        COLORREF          colorRef    { Convert2COLORREF(m_pLut->Get(colIndex)) };
            if (ColorDialog(GetWindowHandle(), colorRef))
            {
                if (ColorLUT::BasePoint * pBasePoint { bit_cast<ColorLUT::BasePoint*>(lParam) })
                {
                    pBasePoint->col = Color(colorRef);
                    m_pLut->Construct();
	    	        Trigger();   // cause repaint
                }
            }
        }
		break;

	case IDM_REMOVE_SUPPORT_PNT:
        if (ColorLUT::BasePoint * pBasePoint{ bit_cast<ColorLUT::BasePoint*>(lParam) })
        {
            m_pLut->RemoveBasePoint(pBasePoint);
            m_pLut->Construct();
            m_pBpSelected = nullptr;
            Trigger();   // cause repaint
        }
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
	m_pBpSelected = nullptr;
	Trigger();   // cause repaint
	GraphicsWindow::OnMouseLeave();
}
