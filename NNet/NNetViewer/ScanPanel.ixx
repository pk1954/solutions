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
    unique_ptr<EventViewer> upEventViewer;
    unique_ptr<NNetWindow>  upScanWindow;
};
