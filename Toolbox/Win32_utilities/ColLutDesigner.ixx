// ColLutDesigner.ixx
//
// Toolbox\win32_utilities

module;

#include <Windows.h> 

export module ColLutDesigner;

import std;
import SaveCast;
import ColorLUT;
import Direct2D;
import GraphicsWindow;

export class ColLutDesigner : public GraphicsWindow
{
public:
    void Start(HWND const, ColorLUT* const);

private:
    ColorLUT  * m_pLut       { nullptr };
    BasePointNr m_bpSelected { NoBasePoint };

    void PaintGraphics() final;

    wstring GetCaption() const final { return L"Color Lookup Table"; }
    
	LPARAM AddContextMenuEntries(HMENU const) final;

    void OnMouseLeave     () final;
    void OnMouseMove      (WPARAM const, LPARAM const);
	bool OnLButtonDown    (WPARAM const, LPARAM const) final;
	bool OnLButtonUp      (WPARAM const, LPARAM const) final;	
    void OnLButtonDblClick(WPARAM const, LPARAM const) final;
	bool OnCommand        (WPARAM const, LPARAM const, PixelPoint const) final;

    fPixel      colIndex2pos      (ColIndex    const) const;
    ColIndex    pos2ColIndex      (fPixel      const) const;
    fPixel      xPos              (BasePointNr const) const;
    fPixelPoint handlePos         (BasePointNr const) const;
    void        editSupportPoint  (BasePointNr const);
    void        removeSupportPoint(BasePointNr const);
    void        moveSupportPoint  (BasePointNr const, ColIndex const);
    BasePointNr addSupportPoint   (fPixelPoint const&);
};