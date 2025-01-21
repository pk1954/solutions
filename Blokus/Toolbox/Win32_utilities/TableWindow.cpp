// TableWindow.cpp : 
//
// Win32_utilities

module TableWindow;
 
using std::function;
using std::vector;
using std::wstring;
using std::ranges::fill;

HWND TableWindow::Initialize
(
	HWND    const hwndParent,
	LPCTSTR const szClass,
	DWORD   const dwWindowStyle
)
{
	HWND hwnd = GraphicsWindow::Initialize
	(
		hwndParent,
		szClass,
		dwWindowStyle
	);
	m_upGraphics = D2D_driver::Create(hwnd);
	return hwnd;
}

void TableWindow::Clear()
{
    fill(m_fPixColWidths, 0._fPixel);
    m_fPixMaxRowHeight = 0._fPixel;
    m_table.Set(L"");
}
    
void TableWindow::CalcRowsAndCols()
{
    m_fPixColWidths.resize(m_table.Width(), 0._fPixel);
    m_table.Size().VisitAllRasterPointsC
    (
        [this](RasterPoint const &pos)
        {
            fPixelRectSize fPixSize { m_upGraphics->CalcRect(m_table.Get(pos)) };
            fPixSize += fPixelRectSize(20._fPixel, 10._fPixel);
            if (fPixSize.GetX() > m_fPixColWidths.at(pos.m_x))
                m_fPixColWidths.at(pos.m_x) = fPixSize.GetXvalue();
            if (fPixSize.GetY() > m_fPixMaxRowHeight)
                m_fPixMaxRowHeight = fPixSize.GetYvalue();
        }
    );
    if (m_fPixMaxRowHeight < m_fPixMinRowHeight)
        m_fPixMaxRowHeight = m_fPixMinRowHeight;
}
    
void TableWindow::PaintGraphics()
{
    fPixelRect fPixRect;
    fPixRect.SetHeight(m_fPixMaxRowHeight);
    for (int y = 0; y < m_table.Height(); ++y)
    {
        for (int x = 0; x < m_table.Width(); ++x)
        {
            fPixel const fPixWidth { m_fPixColWidths.at(x) };
            fPixRect.SetWidth(fPixWidth);
            m_upGraphics->DisplayText(fPixRect, m_table.Get(RasterPoint(x,y)));
            fPixRect.Move2Horz(fPixWidth);
        }
        fPixRect.Move2Vert(m_fPixMaxRowHeight);
        fPixRect.SetLeft(0._fPixel);
    }
}


