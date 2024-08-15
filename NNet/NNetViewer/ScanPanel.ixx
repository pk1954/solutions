// ScanPanel.ixx
//
// NNetViewer

export module ScanPanel;

import std;
import BaseWindow;
import NNetSignals;
import NNetModel;
import NNetWin32;
import EventViewer;
import ScanViewer;

using std::unique_ptr;

export class ScanPanel : public BaseWindow
{
public:
    ScanPanel(HWND const, unique_ptr<Model> model, PIXEL const, mV const&, mV const&);
    ~ScanPanel()
    {
        int x = 42;
    }

	LPARAM AddContextMenuEntries(HMENU const) final;

    PIXEL PanelWidthFromHeight(PIXEL const) const;
    PIXEL PanelHeightFromWidth(PIXEL const) const;

    void Hide();

    unique_ptr<EventViewer> m_upEventViewer;
    unique_ptr<ScanViewer>  m_upScanViewer;

private:
	bool OnCommand(WPARAM const, LPARAM const, PixelPoint const) final;
	bool OnSize   (PIXEL const, PIXEL const)                     final;
    void OnPaint  ()                                             final;

    NNetModelReaderInterface m_nmri;
	unique_ptr<Model>        m_upModel;
    PIXEL                    m_pixFrame;
};
