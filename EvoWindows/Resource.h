#pragma once

// identifiers below 100 are reserved for win32_utilities
#include "win32_util_resource.h"

// identifiers for application start at 100

#define IDI_EVOLUTION                           111
#define IDI_SMALL                               112

#define IDM_RESET_DIALOG                        120
#define IDM_SCRIPT_DIALOG                       121

#define IDM_REFRESH                             140
#define IDM_SET_POI                             144
#define IDM_ESCAPE                              145
#define IDD_TOGGLE_STRIP_MODE                   146
#define IDD_TOGGLE_CLUT_MODE                    147
#define IDD_TOGGLE_COORD_DISPLAY                148
#define IDM_RESET                               149
#define IDM_SOFT_RESET                          150
#define IDM_HISTORY_RESET                       151
#define IDM_HARD_RESET                          152
#define IDM_CHANGE_GRID_TYPE                    153
#define IDM_CHANGE_GRID_TYPE_WIDTH              154
#define IDM_CHANGE_GRID_TYPE_HEIGHT             155
#define IDM_GRID_TYPE_RECT                      156
#define IDM_GRID_TYPE_HEX                       157

#define IDM_CHOOSE_SELECTION_COLOR              161
#define IDM_CHOOSE_HIGHLIGHT_COLOR              162
#define IDM_CHOOSE_STRATEGY_COLOR               163

#define IDC_EVOLUTION_MAIN                      202
#define IDD_EDITOR                              207
#define IDD_DISPLAY_OPTIONS                     208

#define IDM_WINDOWS_BASE                        400
#define IDM_CONS_WINDOW                         (IDM_WINDOWS_BASE +  1)
#define IDM_APPL_WINDOW                         (IDM_WINDOWS_BASE +  2)
#define IDM_MINI_WINDOW                         (IDM_WINDOWS_BASE +  3)
#define IDM_DISP_WINDOW                         (IDM_WINDOWS_BASE +  4)
#define IDM_EDIT_WINDOW                         (IDM_WINDOWS_BASE +  5)
#define IDM_STAT_WINDOW                         (IDM_WINDOWS_BASE +  6)
#define IDM_HIST_WINDOW                         (IDM_WINDOWS_BASE +  7)
#define IDM_CRSR_WINDOW                         (IDM_WINDOWS_BASE +  8)
#define IDM_HIST_INFO                           (IDM_WINDOWS_BASE +  9)
#define IDM_PERF_WINDOW                         (IDM_WINDOWS_BASE + 10)
#define IDM_MAIN_WINDOW                         (IDM_WINDOWS_BASE + 11)
#define IDM_STATUS_BAR                          (IDM_WINDOWS_BASE + 12)

#define IDM_INDIVIDUALS                         450

#define IDM_EDIT_SHAPE_BASE                     500
#define IDM_EDIT_CIRCLE                         (IDM_EDIT_SHAPE_BASE + 1)
#define IDM_EDIT_RECTANGLE                      (IDM_EDIT_SHAPE_BASE + 2)
#define IDM_EDIT_GRID_AREA                      (IDM_EDIT_SHAPE_BASE + 3)

#define IDM_EDIT_OPERATION_BASE                 550
#define IDM_EDIT_OPERATION_SET                  (IDM_EDIT_OPERATION_BASE + 1)
#define IDM_EDIT_OPERATION_MIN                  (IDM_EDIT_OPERATION_BASE + 2)
#define IDM_EDIT_OPERATION_MAX                  (IDM_EDIT_OPERATION_BASE + 3)
#define IDM_EDIT_OPERATION_ADD                  (IDM_EDIT_OPERATION_BASE + 4)
#define IDM_EDIT_OPERATION_SUBTRACT             (IDM_EDIT_OPERATION_BASE + 5)

#define IDM_EDIT_MODE                           600
#define IDM_MOVE                                (IDM_EDIT_MODE +  1)
#define IDM_RANDOM_STRATEGY                     (IDM_EDIT_MODE +  2)
#define IDM_COOPERATE                           (IDM_EDIT_MODE +  3)
#define IDM_DEFECT                              (IDM_EDIT_MODE +  4)
#define IDM_TIT4TAT                             (IDM_EDIT_MODE +  5)
#define IDM_KILL_ANIMALS                        (IDM_EDIT_MODE +  6)
#define IDM_MUT_RATE                            (IDM_EDIT_MODE +  7)
#define IDM_FERTILITY                           (IDM_EDIT_MODE +  8)
#define IDM_FOOD_STOCK                          (IDM_EDIT_MODE +  9)
#define IDM_FERTILIZER                          (IDM_EDIT_MODE + 10)
#define IDM_DSP_ENV_NOTHING                     (IDM_EDIT_MODE + 11)
#define IDM_EDIT_SIZE                           (IDM_EDIT_MODE + 31)
#define IDM_EDIT_INTENSITY                      (IDM_EDIT_MODE + 32)

#define IDM_ZOOM_MODE                           850
#define IDM_FIT_ZOOM                            (IDM_ZOOM_MODE + 1)
#define IDM_SET_ZOOM                            (IDM_ZOOM_MODE + 2)
#define IDM_ZOOM_IN                             (IDM_ZOOM_MODE + 3)
#define IDM_ZOOM_OUT                            (IDM_ZOOM_MODE + 4)
#define IDM_ZOOM_TRACKBAR                       (IDM_ZOOM_MODE + 5)

#define IDM_GOTO_CMD_BASE                       900
#define IDM_GOTO_ORIGIN                         (IDM_GOTO_CMD_BASE + 1)
#define IDM_GOTO_DEATH                          (IDM_GOTO_CMD_BASE + 2)
