#pragma once

//lint -e1923    macro could become const variable

#ifndef IDC_STATIC
#define IDC_STATIC (-1)
#endif

#define IDS_APP_TITLE                           101
#define IDD_ABOUTBOX                            110
#define IDI_EVOLUTION                           111
#define IDI_SMALL                               112

#define IDM_SCRIPT_DIALOG                       120

#define IDM_ABOUT                               130
#define IDM_EXIT                                131
#define IDM_TOGGLE_EDIT_SIMU_MODE               132
#define IDM_SET_POI                             144
#define IDM_ESCAPE                              145
#define IDD_TOGGLE_STRIP_MODE                   146
#define IDD_TOGGLE_CLUT_MODE                    147
#define IDM_RESET                               148

#define IDC_EVOLUTION_MAIN                      202
#define IDD_EDITOR                              207
#define IDD_DISPLAY_OPTIONS                     208
#define IDM_DSP_ENV_NOTHING                     300

#define IDM_WINDOWS_BASE                        400
#define IDM_APPL_WINDOW                         (IDM_WINDOWS_BASE +  1)
#define IDM_MINI_WINDOW                         (IDM_WINDOWS_BASE +  2)
#define IDM_DISP_WINDOW                         (IDM_WINDOWS_BASE +  3)
#define IDM_EDIT_WINDOW                         (IDM_WINDOWS_BASE +  4)
#define IDM_STAT_WINDOW                         (IDM_WINDOWS_BASE +  5)
#define IDM_HIST_WINDOW                         (IDM_WINDOWS_BASE +  6)
#define IDM_CRSR_WINDOW                         (IDM_WINDOWS_BASE +  7)
#define IDM_PERF_WINDOW                         (IDM_WINDOWS_BASE +  8)
#define IDM_MAIN_WINDOW                         (IDM_WINDOWS_BASE +  9)
#define IDM_STATUS_BAR                          (IDM_WINDOWS_BASE + 10)

#define IDM_ANIMALS                             450

#define IDM_EDIT_SHAPE                          500
#define IDM_EDIT_CIRCLE                         (IDM_EDIT_SHAPE + 1)
#define IDM_EDIT_RECTANGLE                      (IDM_EDIT_SHAPE + 2)

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

#define IDM_EDIT_PROPERTY_BASE                  700
#define IDM_EDIT_SIZE                           (IDM_EDIT_PROPERTY_BASE + 1)
#define IDM_EDIT_INTENSITY                      (IDM_EDIT_PROPERTY_BASE + 2)
#define IDM_EDIT_UNDO                           (IDM_EDIT_PROPERTY_BASE + 3)
#define IDM_EDIT_REDO                           (IDM_EDIT_PROPERTY_BASE + 4)

#define IDM_SIMULATION_MODE                     800
#define IDM_GENERATION                          (IDM_SIMULATION_MODE +  1)
#define IDM_BACKWARDS                           (IDM_SIMULATION_MODE +  2)
#define IDM_RUN                                 (IDM_SIMULATION_MODE +  3)
#define IDM_STOP                                (IDM_SIMULATION_MODE +  4)
#define IDM_SIMULATION_SPEED                    (IDM_SIMULATION_MODE +  5)
#define IDM_MAX_SPEED                           (IDM_SIMULATION_MODE +  6)
#define IDM_SPEED_TRACKBAR                      (IDM_SIMULATION_MODE +  7)

#define IDM_ZOOM_MODE                           850
#define IDM_FIT_ZOOM                            (IDM_ZOOM_MODE + 1)
#define IDM_SET_ZOOM                            (IDM_ZOOM_MODE + 2)
#define IDM_ZOOM_IN                             (IDM_ZOOM_MODE + 3)
#define IDM_ZOOM_OUT                            (IDM_ZOOM_MODE + 4)
#define IDM_ZOOM_TRACKBAR                       (IDM_ZOOM_MODE + 5)

#define IDM_GOTO_CMD_BASE                       900
#define IDM_GOTO_ORIGIN                         (IDM_GOTO_CMD_BASE + 1)
#define IDM_GOTO_DEATH                          (IDM_GOTO_CMD_BASE + 2)
