// Win32_Util_Resource.ixx
//
// win32_utilities

module;

#include <Windows.h>

export module Win32_Util_Resource;

// application defined window messages

export inline int const WM_APP_UI_CALL       { WM_APP };
export inline int const WM_APP_CAPTION       { WM_APP + 1 };
export inline int const WM_APP_SCALE_COMMAND { WM_APP + 2 };

// message ids for WM_APP_SCALE_COMMAND

export inline int const SC_LBUTTONDBLCLK     { 1 };

// identifiers below 100 are reserved for win32_utilities

export inline int const IDM_ABOUT               { 3 };
export inline int const IDM_DOCU                { 4 };
export inline int const IDM_EXIT                { 5 };
export inline int const IDM_BLOCK_UI            { 6 };

export inline int const IDD_REFRESH_RATE_DIALOG { 9 };

export inline int const IDD_EDIT_CTL            { 11 };

export inline int const IDM_WINDOW_ON           { 20 };
export inline int const IDM_WINDOW_OFF          { 21 };
export inline int const IDM_WINDOW_AUTO         { 22 };

export inline int const IDM_APPL_WINDOW         { 30 };
export inline int const IDM_MAIN_WINDOW         { 31 };
export inline int const IDM_STATUS_BAR          { 32 };
export inline int const IDM_HIST_WINDOW         { 33 };
export inline int const IDM_HIST_INFO           { 34 };
export inline int const IDM_CONS_WINDOW         { 35 };
export inline int const IDD_EDITOR              { 36 };
export inline int const IDM_MINI_WINDOW         { 39 };

export inline int const IDM_BEEP                { 40 };
export inline int const IDM_SCALE_UNLOCK        { 41 };
export inline int const IDM_SCALE_LOCK2ZERO     { 42 };

export inline int const IDM_NEXT_SCRIPT_CMD     { 50 };

export inline int const IDM_HIST_BUFFER_FULL    { 60 };

export inline int const IDM_TRACKBAR            { 65 };

export inline int const IDM_ZOOM_MODE           { 70 };
export inline int const IDM_FIT_ZOOM            { 71 };
export inline int const IDM_SET_ZOOM            { 72 };
export inline int const IDM_ZOOM_IN             { 73 };
export inline int const IDM_ZOOM_OUT            { 74 };
export inline int const IDM_ZOOM_TRACKBAR       { 75 };

export inline int const IDM_SIMU_MODE           { 80 };
export inline int const IDM_FORWARD             { 81 };
export inline int const IDM_BACKWARDS           { 82 };
export inline int const IDM_RUN_STOP            { 83 };
export inline int const IDM_EDIT_UNDO           { 86 };
export inline int const IDM_EDIT_REDO           { 87 };
export inline int const IDM_MAX_SPEED           { 88 };
export inline int const IDM_SIMULATION_SPEED    { 89 };

export inline int const IDM_MOVE                { 90 };

// application identifiers start at 100
