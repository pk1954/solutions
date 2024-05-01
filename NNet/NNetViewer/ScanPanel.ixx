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

using std::unique_ptr;

export class ScanPanel : public BaseWindow
{
public:
private:
	bool OnSize(PIXEL const, PIXEL const) final;

    unique_ptr<EventViewer> m_upEventViewer;
    unique_ptr<NNetWindow>  m_upScanWindow;
};
