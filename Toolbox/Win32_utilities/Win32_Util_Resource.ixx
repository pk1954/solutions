// Win32_Util_Resource.ixx
//
// Win32_utilities

export module Win32_Util_Resource;

import WinBasics;

// application defined window messages

export constexpr unsigned int WM_APP_UI_CALL       { WM_APP };
export constexpr unsigned int WM_APP_CAPTION       { WM_APP + 1 };
export constexpr unsigned int WM_APP_SCALE_COMMAND { WM_APP + 2 };

// message ids for WM_APP_SCALE_COMMAND

export inline int const SC_LBUTTONDBLCLK          { 1 };

// identifiers below 100 are reserved for win32_utilities
                                                
export inline int const IDM_ABOUT                 { 3 };
export inline int const IDM_DOCU                  { 4 };
export inline int const IDM_EXIT                  { 5 };
export inline int const IDD_SOUND                 { 7 };
export inline int const IDD_COLOR_MENU            { 8 };
export inline int const IDD_REFRESH_RATE_DIALOG   { 9 };
                                                  
export inline int const IDD_EDIT_CTL              { 11 };
export inline int const IDD_COLOR_CTL             { 12 };
export inline int const IDM_DEFAULT_BK_COLOR      { 13 };
                                                  
export inline int const IDX_FILE_NOT_FOUND        { 15 };
export inline int const IDX_ERROR_IN_FILE         { 16 };
export inline int const IDX_READ_PROGRESS_REPORT  { 17 };
export inline int const IDX_WRITE_PROGRESS_REPORT { 18 };
                                                  
export inline int const IDM_WINDOW_ON             { 20 };
export inline int const IDM_WINDOW_OFF            { 21 };
export inline int const IDM_WINDOW_AUTO           { 22 };
                                                  
export inline int const IDM_SELECT                { 25 };
export inline int const IDM_DESELECT              { 26 };
export inline int const IDM_SELECT_ALL            { 27 };
export inline int const IDM_DELETE                { 28 };
                                                  
export inline int const IDM_APPL_WINDOW           { 30 };
export inline int const IDM_MAIN_WINDOW           { 31 };
export inline int const IDM_STATUS_BAR            { 32 };
export inline int const IDM_CONS_WINDOW           { 35 };
export inline int const IDD_EDITOR                { 36 };
export inline int const IDM_MINI_WINDOW           { 39 };
                                                  
export inline int const IDM_BEEP                  { 40 };
export inline int const IDM_SCALE_UNLOCK          { 41 };
export inline int const IDM_SCALE_LOCK2ZERO       { 42 };
export inline int const IDM_SCALE_OFF             { 43 };
export inline int const IDM_SCALE_ON              { 44 };
export inline int const IDM_SCALE_GRID            { 45 };
export inline int const IDM_SCALE_SNAP2GRID       { 46 };
                                                  
export inline int const IDM_NEXT_SCRIPT_CMD       { 50 };
export inline int const IDM_SLOWER                { 51 };
export inline int const IDM_FASTER                { 52 };
                                                  
export inline int const IDM_TRACKBAR              { 60 };

export inline int const IDD_SCALES_UPDATE         { 61 };
export inline int const IDD_GRID_UPDATE           { 62 };
export inline int const IDM_ZOOM_MODE             { 64 };
export inline int const IDM_FIT_ZOOM              { 65 };
export inline int const IDM_SET_ZOOM              { 66 };
export inline int const IDM_ZOOM_IN               { 67 };
export inline int const IDM_ZOOM_OUT              { 68 };
export inline int const IDM_ZOOM_TRACKBAR         { 69 };
                                                  
export inline int const IDM_SIMU_MODE             { 70 };
export inline int const IDM_FORWARD               { 71 };
export inline int const IDM_BACKWARDS             { 72 };
export inline int const IDM_RUN                   { 73 };
export inline int const IDM_STOP                  { 74 };
export inline int const IDM_EDIT_UNDO             { 76 };
export inline int const IDM_EDIT_REDO             { 77 };
export inline int const IDM_MAX_SPEED             { 78 };
export inline int const IDM_SIMULATION_SPEED      { 80 };
export inline int const IDM_RUN_STOP_NOTIFY       { 81 };
export inline int const IDM_DYNAMIC_MODEL_NOTIFY  { 82 };
export inline int const IDM_PERFORMANCE_NOTIFY    { 83 };
                                                  
export inline int const IDM_MOVE                  { 90 };
export inline int const IDM_APP_DATA_CHANGED      { 91 };
export inline int const IDM_ADD_SUPPORT_PNT       { 92 };
export inline int const IDM_EDIT_SUPPORT_PNT      { 93 };
export inline int const IDM_REMOVE_SUPPORT_PNT    { 94 };

// application identifiers start at 100
