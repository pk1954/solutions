// Resource.ixx
//
// EvoWindows

export module Resource;

// identifiers below 100 are reserved for win32_utilities
// identifiers for application start at 100

export inline int const IDI_EVOLUTION                           { 111 };
export inline int const IDI_SMALL                               { 112 };

export inline int const IDM_RESET_DIALOG                        { 120 };
export inline int const IDM_SCRIPT_DIALOG                       { 121 };

export inline int const IDM_SET_POI                             { 144 };
export inline int const IDM_ESCAPE                              { 145 };
export inline int const IDD_TOGGLE_STRIP_MODE                   { 146 };
export inline int const IDD_TOGGLE_CLUT_MODE                    { 147 };
export inline int const IDD_TOGGLE_COORD_DISPLAY                { 148 };
export inline int const IDM_RESET                               { 149 };
export inline int const IDM_SOFT_RESET                          { 150 };
export inline int const IDM_HISTORY_RESET                       { 151 };
export inline int const IDM_HARD_RESET                          { 152 };
export inline int const IDM_CHANGE_GRID_TYPE                    { 153 };
export inline int const IDM_CHANGE_GRID_TYPE_WIDTH              { 154 };
export inline int const IDM_CHANGE_GRID_TYPE_HEIGHT             { 155 };
export inline int const IDM_GRID_TYPE_RECT                      { 156 };
export inline int const IDM_GRID_TYPE_HEX                       { 157 };
export inline int const IDM_HIST_BUFFER_FULL                    { 158 };

export inline int const IDM_CHOOSE_SELECTION_COLOR              { 161 };
export inline int const IDM_CHOOSE_HIGHLIGHT_COLOR              { 162 };
export inline int const IDM_CHOOSE_STRATEGY_COLOR               { 163 };

export inline int const IDC_EVOLUTION_MAIN                      { 202 };
export inline int const IDD_DISPLAY_OPTIONS                     { 208 };

export inline int const IDM_WINDOWS_BASE                        { 400 };
export inline int const IDM_MINI_WINDOW                         { IDM_WINDOWS_BASE +  3 };
export inline int const IDM_DISP_WINDOW                         { IDM_WINDOWS_BASE +  4 };
export inline int const IDM_EDIT_WINDOW                         { IDM_WINDOWS_BASE +  5 };
export inline int const IDM_STAT_WINDOW                         { IDM_WINDOWS_BASE +  6 };
export inline int const IDM_CRSR_WINDOW                         { IDM_WINDOWS_BASE +  8 };
export inline int const IDM_PERF_WINDOW                         { IDM_WINDOWS_BASE + 10 };
export inline int const IDM_HIST_WINDOW                         { IDM_WINDOWS_BASE + 11 };

export inline int const IDM_INDIVIDUALS                         { 450 };

export inline int const IDM_EDIT_SHAPE_BASE                     { 500 };
export inline int const IDM_EDIT_CIRCLE                         { IDM_EDIT_SHAPE_BASE + 1 };
export inline int const IDM_EDIT_RECTANGLE                      { IDM_EDIT_SHAPE_BASE + 2 };
export inline int const IDM_EDIT_GRID_AREA                      { IDM_EDIT_SHAPE_BASE + 3 };

export inline int const IDM_EDIT_OPERATION_BASE                 { 550 };
export inline int const IDM_EDIT_OPERATION_SET                  { IDM_EDIT_OPERATION_BASE + 1 };
export inline int const IDM_EDIT_OPERATION_MIN                  { IDM_EDIT_OPERATION_BASE + 2 };
export inline int const IDM_EDIT_OPERATION_MAX                  { IDM_EDIT_OPERATION_BASE + 3 };
export inline int const IDM_EDIT_OPERATION_ADD                  { IDM_EDIT_OPERATION_BASE + 4 };
export inline int const IDM_EDIT_OPERATION_SUBTRACT             { IDM_EDIT_OPERATION_BASE + 5 };

export inline int const IDM_EDIT_MODE                           { 600 };
export inline int const IDM_RANDOM_STRATEGY                     { IDM_EDIT_MODE +  2 };
export inline int const IDM_COOPERATE                           { IDM_EDIT_MODE +  3 };
export inline int const IDM_DEFECT                              { IDM_EDIT_MODE +  4 };
export inline int const IDM_TIT4TAT                             { IDM_EDIT_MODE +  5 };
export inline int const IDM_KILL_ANIMALS                        { IDM_EDIT_MODE +  6 };
export inline int const IDM_MUT_RATE                            { IDM_EDIT_MODE +  7 };
export inline int const IDM_FERTILITY                           { IDM_EDIT_MODE +  8 };
export inline int const IDM_FOOD_STOCK                          { IDM_EDIT_MODE +  9 };
export inline int const IDM_FERTILIZER                          { IDM_EDIT_MODE + 10 };
export inline int const IDM_DSP_ENV_NOTHING                     { IDM_EDIT_MODE + 11 };
export inline int const IDM_EDIT_SIZE                           { IDM_EDIT_MODE + 31 };
export inline int const IDM_EDIT_INTENSITY                      { IDM_EDIT_MODE + 32 };

export inline int const IDM_GOTO_CMD_BASE                       { 900 };
export inline int const IDM_GOTO_ORIGIN                         { IDM_GOTO_CMD_BASE + 1 };
export inline int const IDM_GOTO_DEATH                          { IDM_GOTO_CMD_BASE + 2 };
