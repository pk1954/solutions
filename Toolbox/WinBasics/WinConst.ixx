// WinConst.ixx
//
// Toolbox\WinConst

export module WinBasics:WinConst;

import :WinDefs;

extern const UINT  PM_REMOVE_CONST;
extern const UINT  CP_UTF8_CONST;
extern const UINT  FIXED_PITCH_CONST;
extern const UINT  GWL_STYLE_CONST;

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
extern const DWORD WS_EX_TOPMOST_CONST;
extern const DWORD WS_EX_STATICEDGE_CONST;
extern const DWORD WS_VSCROLL_CONST;
extern const DWORD WS_HSCROLL_CONST;

extern const DWORD SW_HIDE_CONST;
extern const DWORD SW_SHOW_CONST;
extern const DWORD SW_RESTORE_CONST;
extern const DWORD SW_SHOWNORMAL_CONST;
extern const DWORD SW_MAXIMIZE_CONST;

extern const DWORD SB_SETTEXT_CONST;
extern const DWORD SB_SETPARTS_CONST;

extern const DWORD SM_CXSIZEFRAME_CONST;
extern const DWORD SM_CYSIZEFRAME_CONST;

extern const HWND  HWND_TOP_CONST;
extern const HWND  HWND_DESKTOP_CONST;
extern const HWND  HWND_TOPMOST_CONST;

extern const DWORD SWP_NOSIZE_CONST;
extern const DWORD SWP_NOMOVE_CONST;
extern const DWORD SWP_SHOWWINDOW_CONST;

extern const WORD  VK_SPACE_CONST;
extern const WORD  VK_BACK_CONST;
extern const WORD  VK_DELETE_CONST;
extern const WORD  VK_ESCAPE_CONST;

extern const DWORD MF_BITMAP_CONST;
extern const DWORD MF_STRING_CONST;
extern const DWORD MF_BYPOSITION_CONST;
extern const DWORD MF_GRAYED_CONST;
extern const DWORD MF_ENABLED_CONST;
extern const DWORD MF_POPUP_CONST;

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

extern const DWORD EN_CHANGE_CONST;
extern const DWORD EM_GETSEL_CONST;
extern const DWORD EM_SETSEL_CONST;
extern const DWORD EM_GETLINECOUNT_CONST;

extern const DWORD ES_RIGHT_CONST;
extern const DWORD ES_MULTILINE_CONST;  
extern const DWORD ES_WANTRETURN_CONST; 
extern const DWORD ES_AUTOHSCROLL_CONST;
extern const DWORD ES_AUTOVSCROLL_CONST;

extern const DWORD TA_CENTER_CONST;
extern const DWORD TA_LEFT_CONST;
extern const DWORD TA_RIGHT_CONST;

extern const DWORD TME_HOVER_CONST;
extern const DWORD TME_LEAVE_CONST;
extern const DWORD TBM_GETPOS_CONST;
extern const DWORD TBM_SETRANGEMIN_CONST;
extern const DWORD TBM_SETRANGEMAX_CONST;
extern const DWORD TBS_NOTICKS_CONST;
extern const DWORD TPM_TOPALIGN_CONST; 
extern const DWORD TPM_LEFTALIGN_CONST;
extern const DWORD TPM_RETURNCMD_CONST;
extern const DWORD TPM_NONOTIFY_CONST;
extern const DWORD TMPF_FIXED_PITCH_CONST;

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

extern const int EXCEPTION_EXECUTE_HANDLER_CONST;

export 
{
    const UINT    TRUE                = 1;
    const UINT    FALSE               = 0;

    const UINT    WM_APP              = 0x8000;
    const UINT    WM_CHAR             = 0x0102;
    const UINT    WM_CLEAR            = 0x0303;
    const UINT    WM_CLOSE            = 0x0010;
    const UINT    WM_COMMAND          = 0x0111;
    const UINT    WM_CTLCOLOREDIT     = 0x0133;
    const UINT    WM_CTLCOLORSTATIC   = 0x0138;
    const UINT    WM_CONTEXTMENU      = 0x007B;
    const UINT    WM_ENTERMENULOOP    = 0x0211;
    const UINT    WM_ERASEBKGND       = 0x0014;
    const UINT    WM_DESTROY          = 0x0002;
    const UINT    WM_DRAWITEM         = 0x002B;
    const UINT    WM_HSCROLL          = 0x0114;
    const UINT    WM_INITDIALOG       = 0x0110;
    const UINT    WM_LBUTTONDBLCLK    = 0x0203;
    const UINT    WM_LBUTTONDOWN      = 0x0201;
    const UINT    WM_LBUTTONUP        = 0x0202;
    const UINT    WM_MENUCOMMAND      = 0x0126;
    const UINT    WM_MOUSEHOVER       = 0x02A1;
    const UINT    WM_MOUSELEAVE       = 0x02A3;
    const UINT    WM_MOUSEMOVE        = 0x0200;
    const UINT    WM_MOUSEWHEEL       = 0x020A;
    const UINT    WM_MOVE             = 0x0003;
    const UINT    WM_NCLBUTTONDBLCLK  = 0x00A3;
    const UINT    WM_NCCREATE         = 0x0081;
    const UINT    WM_NOTIFY           = 0x004E;
    const UINT    WM_PAINT            = 0x000F;
    const UINT    WM_RBUTTONDOWN      = 0x0204;
    const UINT    WM_RBUTTONUP        = 0x0205;
    const UINT    WM_QUIT             = 0x0012;
    const UINT    WM_SETCURSOR        = 0x0020;
    const UINT    WM_SETFONT          = 0x0030;
    const UINT    WM_SHOWWINDOW       = 0x0018;
    const UINT    WM_SIZE             = 0x0005;
    const UINT    WM_TIMER            = 0x0113;

    const DWORD   ERROR_CLASS_ALREADY_EXISTS = 1410L;
    const DWORD   SRCCOPY                    = 0x00CC0020; // dest = source
    const DWORD   CC_RGBINIT                 = 0x00000001;
    const DWORD   CC_FULLOPEN                = 0x00000002;

    const UINT    PM_REMOVE           = PM_REMOVE_CONST;
    const UINT    CP_UTF8             = CP_UTF8_CONST;
    const UINT    FIXED_PITCH         = FIXED_PITCH_CONST;
    const UINT    GWL_STYLE           = GWL_STYLE_CONST;

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
    const DWORD   WS_EX_TOPMOST       = WS_EX_TOPMOST_CONST;
    const DWORD   WS_EX_STATICEDGE    = WS_EX_STATICEDGE_CONST;
    const DWORD   WS_VSCROLL          = WS_VSCROLL_CONST;
    const DWORD   WS_HSCROLL          = WS_HSCROLL_CONST;
                                      
    const DWORD   SW_HIDE             = SW_HIDE_CONST;
    const DWORD   SW_SHOW             = SW_SHOW_CONST;
    const DWORD   SW_RESTORE          = SW_RESTORE_CONST;
    const DWORD   SW_SHOWNORMAL       = SW_SHOWNORMAL_CONST;
    const DWORD   SW_MAXIMIZE         = SW_MAXIMIZE_CONST;
                                  
    const DWORD   SB_SETTEXT          = SB_SETTEXT_CONST;
    const DWORD   SB_SETPARTS         = SB_SETPARTS_CONST;
                                  
    const DWORD   SM_CXSIZEFRAME      = SM_CXSIZEFRAME_CONST;
    const DWORD   SM_CYSIZEFRAME      = SM_CYSIZEFRAME_CONST;

    const HWND    HWND_TOP            = HWND_TOP_CONST;
    const HWND    HWND_DESKTOP        = HWND_DESKTOP_CONST;
    const HWND    HWND_TOPMOST        = HWND_TOPMOST_CONST;
                                      
    const DWORD   SWP_NOSIZE          = SWP_NOSIZE_CONST;
    const DWORD   SWP_NOMOVE          = SWP_NOMOVE_CONST;
    const DWORD   SWP_SHOWWINDOW      = SWP_SHOWWINDOW_CONST;
                                      
    const WORD    VK_SPACE            = VK_SPACE_CONST;
    const WORD    VK_BACK             = VK_BACK_CONST;
    const WORD    VK_DELETE           = VK_DELETE_CONST;
    const WORD    VK_ESCAPE           = VK_ESCAPE_CONST;
                                      
    const DWORD   MF_BITMAP           = MF_BITMAP_CONST;
    const DWORD   MF_STRING           = MF_STRING_CONST;
    const DWORD   MF_BYPOSITION       = MF_BYPOSITION_CONST;
    const DWORD   MF_GRAYED           = MF_GRAYED_CONST;
    const DWORD   MF_ENABLED          = MF_ENABLED_CONST;
    const DWORD   MF_POPUP            = MF_POPUP_CONST;
                                      
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

    const DWORD   EN_CHANGE           = EN_CHANGE_CONST;
    const DWORD   EM_GETSEL           = EM_GETSEL_CONST;
    const DWORD   EM_SETSEL           = EM_SETSEL_CONST;
    const DWORD   EM_GETLINECOUNT     = EM_GETLINECOUNT_CONST;

    const DWORD   ES_RIGHT            = ES_RIGHT_CONST;
    const DWORD   ES_MULTILINE        = ES_MULTILINE_CONST;  
    const DWORD   ES_WANTRETURN       = ES_WANTRETURN_CONST; 
    const DWORD   ES_AUTOHSCROLL      = ES_AUTOHSCROLL_CONST;
    const DWORD   ES_AUTOVSCROLL      = ES_AUTOVSCROLL_CONST;
                  
    const DWORD   TA_CENTER           = TA_CENTER_CONST;
    const DWORD   TA_LEFT             = TA_LEFT_CONST;
    const DWORD   TA_RIGHT            = TA_RIGHT_CONST;
                  
    const DWORD   TME_HOVER           = TME_HOVER_CONST;
    const DWORD   TME_LEAVE           = TME_LEAVE_CONST;
    const DWORD   TBM_GETPOS          = TBM_GETPOS_CONST;
    const DWORD   TBM_SETRANGEMIN     = TBM_SETRANGEMIN_CONST;
    const DWORD   TBM_SETRANGEMAX     = TBM_SETRANGEMAX_CONST;
    const DWORD   TBS_NOTICKS         = TBS_NOTICKS_CONST;
    const DWORD   TPM_TOPALIGN        = TPM_TOPALIGN_CONST; 
    const DWORD   TPM_LEFTALIGN       = TPM_LEFTALIGN_CONST;
    const DWORD   TPM_RETURNCMD       = TPM_RETURNCMD_CONST;
    const DWORD   TPM_NONOTIFY        = TPM_NONOTIFY_CONST;
    const DWORD   TMPF_FIXED_PITCH    = TMPF_FIXED_PITCH_CONST;

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

    const LPCWSTR STATUSCLASSNAME     = L"msctls_statusbar32";
    const LPCWSTR TRACKBAR_CLASS      = L"msctls_trackbar32";

    const int EXCEPTION_EXECUTE_HANDLER = EXCEPTION_EXECUTE_HANDLER_CONST;
}
