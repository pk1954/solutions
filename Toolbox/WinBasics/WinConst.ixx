// WinConst.ixx
//
// Toolbox\WinConst

export module WinBasics:WinConst;

import :WinDefs;

extern const UINT  WM_ENTERMENULOOP_CONST;
extern const UINT  WM_CTLCOLOREDIT_CONST;
extern const UINT  WM_DESTROY_CONST;
extern const UINT  WM_CLOSE_CONST;

extern const DWORD WS_BORDER_CONST;
extern const DWORD WS_CHILD_CONST;
extern const DWORD WS_CLIPSIBLINGS_CONST;
extern const DWORD WS_CLIPCHILDREN_CONST;
extern const DWORD WS_VISIBLE_CONST;
extern const DWORD WS_OVERLAPPEDWINDOW_CONST;
extern const DWORD WS_OVERLAPPED_CONST;
extern const DWORD WS_POPUPWINDOW_CONST;
extern const DWORD WS_POPUP_CONST;
extern const DWORD WS_CAPTION_CONST;
extern const DWORD WS_SIZEBOX_CONST;
extern const DWORD WS_GROUP_CONST;
extern const DWORD WS_TABSTOP_CONST;
extern const DWORD WS_SYSMENU_CONST;

extern const DWORD SW_HIDE_CONST;
extern const DWORD SW_SHOW_CONST;

extern const DWORD SB_SETTEXT_CONST;
extern const DWORD SB_SETPARTS_CONST;

extern const DWORD SM_CXSIZEFRAME_CONST;
extern const DWORD SM_CYSIZEFRAME_CONST;

extern const HWND  HWND_TOP_CONST;
extern const HWND  HWND_DESKTOP_CONST;

extern const DWORD SWP_NOSIZE_CONST;
extern const DWORD SWP_SHOWWINDOW_CONST;

extern const WORD  VK_SPACE_CONST;
extern const WORD  VK_BACK_CONST;
extern const WORD  VK_DELETE_CONST;
extern const WORD  VK_ESCAPE_CONST;

extern const DWORD MF_BITMAP_CONST;
extern const DWORD MF_STRING_CONST;
extern const DWORD MF_BYPOSITION_CONST;

extern const DWORD MB_OK_CONST;
extern const DWORD MB_ICONWARNING_CONST;
extern const DWORD MB_YESNOCANCEL_CONST;
extern const DWORD MB_YESNO_CONST;

extern const DWORD MK_SHIFT_CONST;
extern const DWORD MK_CONTROL_CONST;
extern const DWORD MK_LBUTTON_CONST;

extern const DWORD BS_PUSHBUTTON_CONST;
extern const DWORD BS_DEFPUSHBUTTON_CONST;
extern const DWORD BS_AUTOCHECKBOX_CONST;
extern const DWORD BS_OWNERDRAW_CONST;

extern const DWORD CS_OWNDC_CONST;     
extern const DWORD CS_HREDRAW_CONST;
extern const DWORD CS_VREDRAW_CONST;     
extern const DWORD CS_DBLCLKS_CONST;     

extern const DWORD CW_USEDEFAULT_CONST;

extern const DWORD DS_CENTER_CONST;
extern const DWORD DS_MODALFRAME_CONST;
extern const DWORD DS_SHELLFONT_CONST;

extern const DWORD DT_LEFT_CONST;

extern const DWORD TBS_NOTICKS_CONST;

extern const DWORD ES_RIGHT_CONST;

extern const BYTE  FVIRTKEY_CONST; 
extern const BYTE  FCONTROL_CONST; 
extern const BYTE  FALT_CONST; 

extern const int   IDYES_CONST; 
extern const int   IDNO_CONST; 
extern const int   IDOK_CONST; 
extern const int   IDCANCEL_CONST; 

extern const int   DC_BRUSH_CONST; 
extern const int   DC_DEFBRUSH_CONST; 
                                    
extern const LPCWSTR IDC_SIZENS_CONST;     
extern const LPCWSTR IDC_SIZEWE_CONST;     
extern const LPCWSTR IDC_WAIT_CONST;     
extern const LPCWSTR IDC_ARROW_CONST;     

export 
{
    const UINT    WM_ENTERMENULOOP    = WM_ENTERMENULOOP_CONST;
    const UINT    WM_CTLCOLOREDIT     = WM_CTLCOLOREDIT_CONST;
    const UINT    WM_DESTROY          = WM_DESTROY_CONST;
    const UINT    WM_CLOSE            = WM_CLOSE_CONST;
    const UINT    WM_HSCROLL          = 0x0114;
    const UINT    WM_COMMAND          = 0x0111;
    const UINT    WM_APP              = 0x8000;
                  
    const DWORD   WS_BORDER           = WS_BORDER_CONST;
    const DWORD   WS_CHILD            = WS_CHILD_CONST;
    const DWORD   WS_CLIPSIBLINGS     = WS_CLIPSIBLINGS_CONST;
    const DWORD   WS_CLIPCHILDREN     = WS_CLIPCHILDREN_CONST;
    const DWORD   WS_VISIBLE          = WS_VISIBLE_CONST;
    const DWORD   WS_OVERLAPPEDWINDOW = WS_OVERLAPPEDWINDOW_CONST;
    const DWORD   WS_OVERLAPPED       = WS_OVERLAPPED_CONST;
    const DWORD   WS_POPUPWINDOW      = WS_POPUPWINDOW_CONST;
    const DWORD   WS_POPUP            = WS_POPUPWINDOW;
    const DWORD   WS_CAPTION          = WS_CAPTION_CONST;
    const DWORD   WS_SIZEBOX          = WS_SIZEBOX_CONST;
    const DWORD   WS_GROUP            = WS_GROUP_CONST;
    const DWORD   WS_TABSTOP          = WS_TABSTOP_CONST;
    const DWORD   WS_SYSMENU          = WS_SYSMENU_CONST;
                                      
    const DWORD   SW_HIDE             = SW_HIDE_CONST;
    const DWORD   SW_SHOW             = SW_SHOW_CONST;
                                      
    const DWORD   SB_SETTEXT          = SB_SETTEXT_CONST;
    const DWORD   SB_SETPARTS         = SB_SETPARTS_CONST;
                                  
    const DWORD   SM_CXSIZEFRAME      = SM_CXSIZEFRAME_CONST;
    const DWORD   SM_CYSIZEFRAME      = SM_CYSIZEFRAME_CONST;

    const HWND    HWND_TOP            = HWND_TOP_CONST;
    const HWND    HWND_DESKTOP        = HWND_DESKTOP_CONST;
                                      
    const DWORD   SWP_NOSIZE          = SWP_NOSIZE_CONST;
    const DWORD   SWP_SHOWWINDOW      = SWP_SHOWWINDOW_CONST;
                                      
    const WORD    VK_SPACE            = VK_SPACE_CONST;
    const WORD    VK_BACK             = VK_BACK_CONST;
    const WORD    VK_DELETE           = VK_DELETE_CONST;
    const WORD    VK_ESCAPE           = VK_ESCAPE_CONST;
                                      
    const DWORD   MF_BITMAP           = MF_BITMAP_CONST;
    const DWORD   MF_STRING           = MF_STRING_CONST;
    const DWORD   MF_BYPOSITION       = MF_BYPOSITION_CONST;
                                      
    const DWORD   MB_OK               = MB_OK_CONST;
    const DWORD   MB_ICONWARNING      = MB_ICONWARNING_CONST;
    const DWORD   MB_YESNOCANCEL      = MB_YESNOCANCEL_CONST;
    const DWORD   MB_YESNO            = MB_YESNO_CONST;
                                      
    const DWORD   MK_SHIFT            = MK_SHIFT_CONST;
    const DWORD   MK_CONTROL          = MK_CONTROL_CONST;
    const DWORD   MK_LBUTTON          = MK_LBUTTON_CONST;
                                      
    const DWORD   BS_PUSHBUTTON       = BS_PUSHBUTTON_CONST;
    const DWORD   BS_DEFPUSHBUTTON    = BS_DEFPUSHBUTTON_CONST;
    const DWORD   BS_AUTOCHECKBOX     = BS_AUTOCHECKBOX_CONST;
    const DWORD   BS_OWNERDRAW        = BS_OWNERDRAW_CONST;
                                      
    const DWORD   CS_OWNDC            = CS_OWNDC_CONST;     
    const DWORD   CS_HREDRAW          = CS_HREDRAW_CONST;
    const DWORD   CS_VREDRAW          = CS_VREDRAW_CONST;     
    const DWORD   CS_DBLCLKS          = CS_DBLCLKS_CONST; 
                  
    const DWORD   CW_USEDEFAULT       = CW_USEDEFAULT_CONST; 
                  
    const DWORD   DS_CENTER           = DS_CENTER_CONST;
    const DWORD   DS_MODALFRAME       = DS_MODALFRAME_CONST;
    const DWORD   DS_SHELLFONT        = DS_SHELLFONT_CONST;
                  
    const DWORD   DT_LEFT             = DT_LEFT_CONST;

    const DWORD   TBS_NOTICKS         = TBS_NOTICKS_CONST;

    const DWORD   ES_RIGHT            = ES_RIGHT_CONST;
                  
    const BYTE    FVIRTKEY            = FVIRTKEY_CONST; 
    const BYTE    FCONTROL            = FCONTROL_CONST; 
    const BYTE    FALT                = FALT_CONST; 
                                      
    const int     IDYES               = IDYES_CONST; 
    const int     IDNO                = IDNO_CONST; 
    const int     IDOK                = IDOK_CONST; 
    const int     IDCANCEL            = IDCANCEL_CONST; 
                                      
    const int     DC_BRUSH            = DC_BRUSH_CONST; 
                                      
    const LPCWSTR IDC_SIZENS          = IDC_SIZENS_CONST; 
    const LPCWSTR IDC_SIZEWE          = IDC_SIZEWE_CONST; 
    const LPCWSTR IDC_WAIT            = IDC_WAIT_CONST; 
    const LPCWSTR IDC_ARROW           = IDC_ARROW_CONST; 

    const LPCWSTR WC_BUTTON           = L"Button";
    const LPCWSTR WC_EDIT             = L"Edit";
    const LPCWSTR WC_STATIC           = L"Static";

    const LPCWSTR STATUSCLASSNAME     = L"msctls_statusbar";
    const LPCWSTR TRACKBAR_CLASS      = L"msctls_trackbar32";
}
