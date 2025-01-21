// TableWindow.cpp : 
//
// Win32_utilities

export module TableWindow;

import std;
import Types;
import Raster;
import Vector2D;
import GraphicsWindow;

using std::vector;
using std::wstring;
using std::to_wstring;

export class TableWindow : public GraphicsWindow
{
public:

    TableWindow() = default;

    HWND Initialize(HWND const, LPCTSTR const, DWORD const);

    void Resize(RasterPoint const& rpSize)
    {
        m_table.Resize(rpSize);
    }

    void Resize(int const rows, int const cols)
    {
        Resize(RasterPoint(rows, cols));
    }

    void Set(RasterPoint const& rp, wstring const& text)
    {
        m_table.Set(rp, text);
    }

    void Set(RasterPoint const& rp, int const iVal)
    {
        Set(rp, to_wstring(iVal));
    }

    void SetMinRowHeight(fPixel const h)
    {
        m_fPixMinRowHeight = h;
    }

    void Clear();
    void CalcRowsAndCols();
    void PaintGraphics() override;

private:
 
    Vector2D<wstring> m_table;
    vector<fPixel>    m_fPixColWidths;
    fPixel            m_fPixMaxRowHeight { 0._fPixel };
    fPixel            m_fPixMinRowHeight { 0._fPixel };
};