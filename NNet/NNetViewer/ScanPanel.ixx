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
    ScanPanel(HWND const, NNetModelWriterInterface* const);

    float AspectRatio() const;

private:
	bool OnSize(PIXEL const, PIXEL const) final;

    unique_ptr<EventViewer> m_upEventViewer;
    unique_ptr<ScanViewer>  m_upScanViewer;
};
