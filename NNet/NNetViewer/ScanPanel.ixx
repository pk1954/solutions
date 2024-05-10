// ScanPanel.ixx
//
// NNetViewer

module;

#include <memory>

export module ScanPanel;

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
    ScanPanel(HWND const, unique_ptr<Model> model);

    float AspectRatio() const;

private:
	bool OnSize(PIXEL const, PIXEL const) final;

    NNetModelReaderInterface m_nmri;
	unique_ptr<Model>        m_upModel;
    unique_ptr<EventViewer>  m_upEventViewer;
    unique_ptr<ScanViewer>   m_upScanViewer;
};
