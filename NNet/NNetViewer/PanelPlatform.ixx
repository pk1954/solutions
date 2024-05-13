// PanelPlatform.ixx
//
// NNetViewer

module;

#include <memory>
#include <vector>

export module PanelPlatform;

import Types;
import BaseWindow;
import ScanPanel;
import NNetModel;

using std::unique_ptr;
using std::vector;

export using UpPanel = unique_ptr<ScanPanel>;

export class PanelPlatform :public BaseWindow
{
public:
    PanelPlatform(HWND const);

    void AddScan(unique_ptr<Model>);

private:

   bool OnSize(PIXEL const, PIXEL const) final;
   void OnPaint()                        final;

   void                      arrangePanels(PixelRectSize const&);
   vector<UpPanel>::iterator findPanel(PixelPoint const &);

   vector<UpPanel> m_panelList;
   long            m_nrOfRows { 0 };
   long            m_nrOfCols { 0 };
   PIXEL           m_pixPanelWidth;
   PIXEL           m_pixPanelHeight;
};
