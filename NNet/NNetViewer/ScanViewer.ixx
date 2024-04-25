// ScanViewer.ixx
//
// NNetViewer

module;

#include <memory>
#include <vector>

export module NNetWin32:ScanViewer;

import BaseWindow;
import :ScanPanel;

using std::unique_ptr;
using std::vector;

export class ScanViewer : public BaseWindow
{
public:
    void AddScan()
    {

    }

private:
    vector<unique_ptr<ScanPanel>> m_panelList;
};
