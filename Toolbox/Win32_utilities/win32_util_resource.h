// win32_util_resource.h
//
// win32_utilities

#pragma once

#include <Windows.h>
#include <tchar.h>

#ifndef IDC_STATIC
#define IDC_STATIC				-1
#endif

// application defined window messages

#define WM_APP_UI_CALL         WM_APP
#define WM_APP_CAPTION        (WM_APP + 1)
#define WM_APP_SCALE_COMMAND  (WM_APP + 2)

// message ids for WM_APP_SCALE_COMMAND

#define SC_LBUTTONDBLCLK   1 

// identifiers below 100 are reserved for win32_utilities

#define IDM_ABOUT                       4
#define IDM_EXIT                        5
#define IDM_BLOCK_UI                    6

#define IDD_REFRESH_RATE_DIALOG         9

#define IDD_STD_EDIT_DIALOG            10
#define IDD_EDIT_CTL                   11

#define IDM_WINDOW_ON                  20
#define IDM_WINDOW_OFF                 21
#define IDM_WINDOW_AUTO                22

#define IDM_APPL_WINDOW                30
#define IDM_MAIN_WINDOW                31
#define IDM_STATUS_BAR                 32
#define IDM_HIST_WINDOW                33
#define IDM_HIST_INFO                  34
#define IDM_CONS_WINDOW                35
#define IDD_EDITOR                     36
#define IDM_MINI_WINDOW                39

#define IDM_BEEP                       40
#define IDM_SCALE_UNLOCK               41
#define IDM_SCALE_LOCK2ZERO            42

#define IDM_NEXT_SCRIPT_CMD            50

#define IDM_HIST_BUFFER_FULL           60

#define IDM_TRACKBAR                   65    

#define IDM_ZOOM_MODE                  70      
#define IDM_FIT_ZOOM                   71      
#define IDM_SET_ZOOM                   72    
#define IDM_ZOOM_IN                    73    
#define IDM_ZOOM_OUT                   74    
#define IDM_ZOOM_TRACKBAR              75    

#define IDM_SIMU_MODE                  80
#define IDM_FORWARD                    81
#define IDM_BACKWARDS                  82
#define IDM_RUN_STOP                   83
#define IDM_EDIT_UNDO                  86
#define IDM_EDIT_REDO                  87
#define IDM_MAX_SPEED                  88
#define IDM_SIMULATION_SPEED           89

#define IDM_MOVE                       90

// application identifiers start at 100
