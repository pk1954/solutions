// WinConst.ixx
//
// Toolbox\WinConst

export module WinBasics:WinConst;

import :WinDefs;

extern const DWORD WS_CHILD_CONST;
extern const DWORD WS_CLIPSIBLINGS_CONST;
extern const DWORD WS_CLIPCHILDREN_CONST;
extern const DWORD WS_VISIBLE_CONST;
extern const DWORD WS_OVERLAPPEDWINDOW_CONST;
extern const DWORD WS_POPUPWINDOW_CONST;
extern const DWORD WS_CAPTION_CONST;
extern const DWORD WS_SIZEBOX_CONST;

extern const DWORD SW_HIDE_CONST;

extern const DWORD VK_SPACE_CONST;

extern const DWORD MF_STRING_CONST;

extern const DWORD MB_OK_CONST;
extern const DWORD MB_ICONWARNING_CONST;

extern const DWORD MK_SHIFT_CONST;
extern const DWORD MK_CONTROL_CONST;
extern const DWORD MK_LBUTTON_CONST;

extern const DWORD BS_PUSHBUTTON_CONST;

extern const DWORD CS_OWNDC_CONST;     
extern const DWORD CS_HREDRAW_CONST;
extern const DWORD CS_VREDRAW_CONST;     
extern const DWORD CS_DBLCLKS_CONST;     

extern const LPCWSTR IDC_SIZENS_CONST;     
extern const LPCWSTR IDC_SIZEWE_CONST;     

export 
{
    const DWORD WS_CHILD            = WS_CHILD_CONST;
    const DWORD WS_CLIPSIBLINGS     = WS_CLIPSIBLINGS_CONST;
    const DWORD WS_CLIPCHILDREN     = WS_CLIPCHILDREN_CONST;
    const DWORD WS_VISIBLE          = WS_VISIBLE_CONST;
    const DWORD WS_OVERLAPPEDWINDOW = WS_OVERLAPPEDWINDOW_CONST;
    const DWORD WS_POPUPWINDOW      = WS_POPUPWINDOW_CONST;
    const DWORD WS_CAPTION          = WS_CAPTION_CONST;
    const DWORD WS_SIZEBOX          = WS_SIZEBOX_CONST;

    const DWORD SW_HIDE             = SW_HIDE_CONST;
                                   
    const DWORD VK_SPACE            = VK_SPACE_CONST;
                                   
    const DWORD MF_STRING           = MF_STRING_CONST;
                                   
    const DWORD MB_OK               = MB_OK_CONST;
    const DWORD MB_ICONWARNING      = MB_ICONWARNING_CONST;
                                   
    const DWORD MK_SHIFT            = MK_SHIFT_CONST;
    const DWORD MK_CONTROL          = MK_CONTROL_CONST;
    const DWORD MK_LBUTTON          = MK_LBUTTON_CONST;
                                   
    const DWORD BS_PUSHBUTTON       = BS_PUSHBUTTON_CONST;
                                   
    const DWORD CS_OWNDC            = CS_OWNDC_CONST;     
    const DWORD CS_HREDRAW          = CS_HREDRAW_CONST;
    const DWORD CS_VREDRAW          = CS_VREDRAW_CONST;     
    const DWORD CS_DBLCLKS          = CS_DBLCLKS_CONST; 

    const LPCWSTR IDC_SIZENS        = IDC_SIZENS_CONST; 
    const LPCWSTR IDC_SIZEWE        = IDC_SIZEWE_CONST; 
}
