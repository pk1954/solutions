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

   struct PANEL_RECT
   {
       PixelRect m_rect;

       PANEL_RECT& operator+=(PANEL_RECT const &rhs)
       {
	       m_rect.Move(rhs.m_rect.GetStartPoint());
           return *this; 
       }

       PANEL_RECT& operator-=(PANEL_RECT const &rhs)
       {
	       m_rect.Move(-rhs.m_rect.GetStartPoint());
           return *this; 
       }

       PANEL_RECT& operator*=(float const factor)
       {
           fPixelPoint const fPixPos { Convert2fPixelPoint(m_rect.GetStartPoint())};
           PixelPoint  const pixPos  { Convert2PixelPoint(fPixPos * factor) };
           m_rect.SetPos(pixPos);
           return *this; 
       }
   };

   struct PANEL_RECT_LIST
   {
       vector<PANEL_RECT> m_list;

       PANEL_RECT_LIST& operator+= (PANEL_RECT_LIST const &rhs) 
       { 
           for (int i = 0; i < m_list.size(); ++i)
	           m_list[i] += rhs.m_list[i];
           return *this; 
       }

       PANEL_RECT_LIST& operator-= (PANEL_RECT_LIST const &rhs) 
       { 
           for (int i = 0; i < m_list.size(); ++i)
	           m_list[i] -= rhs.m_list[i];
           return *this; 
       }

       PANEL_RECT_LIST& operator*= (float const factor) 
       { 
           for (int i = 0; i < m_list.size(); ++i)
	           m_list[i] *= factor;
           return *this; 
       }
   };

   bool OnCommand(WPARAM const, LPARAM const, PixelPoint const) final;
   bool OnSize   (PIXEL const, PIXEL const)                     final;
   void OnPaint  ()                                             final;

   void calcTargetPanelList();
   void arrangePanels();
   void removeScan(ScanPanel *);
   void recalc();

   vector<UpPanel> m_upPanels;
   PANEL_RECT_LIST m_panelRects;

   mV    m_mVmaxPixel     { 0.0_mV };
   mV    m_mVmaxAmplitude { 0.0_mV };
   long  m_nrOfRows       { 0 };
   long  m_nrOfCols       { 0 };
   PIXEL m_pixPanelWidth;
   PIXEL m_pixPanelHeight;
};
