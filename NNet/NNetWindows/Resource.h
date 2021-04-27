// Resource.h
// NNetSimu
// Used by NNetSimu.rc

#pragma once

#include "win32_util_resource.h"

// identifiers below 100 are reserved for win32_utilities
// application identifiers start at 100

#define IDM_FATAL_ERROR     	 100
								 
#define IDC_NNET_SIMU_MAIN  	 102
#define IDD_APPLY_PARAMETERS     105
#define IDD_RESET_PARAMETERS     106

#define IDM_SCRIPT_DIALOG        110
#define IDM_UNDO                 115
#define IDM_REDO                 116

#define IDI_NNETSIMU			 120
#define IDI_SMALL				 121
#define IDC_NNETSIMU			 122

#define IDM_SLOWER               130
#define IDM_FASTER               131

#define IDM_ESCAPE               140
#define IDM_RESET                141
#define IDM_SELECT_ALL           142
#define IDM_SELECT_SUBTREE       143
#define IDM_SELECT_SHAPE         144
#define IDM_DESELECT_ALL         145
#define IDM_DESELECT_SUBTREE     146
#define IDM_DESELECT_SHAPE       147
#define IDM_SELECTION            148
#define IDM_DELETE               149

#define IDM_SAVE_MODEL           150
#define IDM_ASK_AND_SAVE_MODEL   151
#define IDM_SAVE_MODEL_AS        152
#define IDM_OPEN_MODEL           153
#define IDM_ADD_MODEL            154
#define IDX_REPLACE_MODEL        155
#define IDM_NEW_MODEL            156
#define IDM_ADD_IMPORTED_MODEL   157

#define IDD_PULSE_RATE           160
#define IDD_ARROWS_ON            161
#define IDD_ARROWS_OFF           162
#define IDD_RULER_ON             163
#define IDD_RULER_OFF            164
#define IDD_MEASUREMENT_ON       165
#define IDD_MEASUREMENT_OFF      166
#define IDD_ADD_TRACK            168
#define IDD_DELETE_TRACK         169

#define IDD_SOUND_ON             171
#define IDD_SOUND_OFF            172
#define IDX_PLAY_SOUND           173

#define IDD_AUTO_OPEN_ON         181
#define IDD_AUTO_OPEN_OFF        182

#define IDM_PLUS                 191
#define IDM_MINUS                192

#define IDM_MONITOR_WINDOW       530
#define IDD_ADD_SIGNAL           545
#define IDD_DELETE_SIGNAL        546
#define IDD_CONNECT              550

#define IDD_INSERT_KNOT          560
#define IDD_INSERT_NEURON        561
#define IDD_NEW_NEURON           562
#define IDD_NEW_INPUT_NEURON     563
#define IDD_NEW_OUTPUT_NEURON    564
#define IDD_APPEND_NEURON        565
#define IDD_APPEND_INPUT_NEURON  566
#define IDD_DELETE_SHAPE         567
#define IDD_DISCONNECT           568
#define IDD_ADD_INCOMING2KNOT    569
#define IDD_ADD_INCOMING2PIPE    570
#define IDD_ADD_OUTGOING2KNOT    571
#define IDD_ADD_OUTGOING2PIPE    572
#define IDD_TRIGGER_SOUND_DLG    573
#define IDC_TRIGGER_SOUND_ON     574
#define IDC_TRIGGER_SOUND_FREQ   575
#define IDC_TRIGGER_SOUND_MSEC   576
#define IDC_TRIGGER_SOUND_TEST   577
#define IDD_STOP_ON_TRIGGER      579

#define IDM_CRSR_WINDOW          580
#define IDM_PARAM_WINDOW  	     581
#define IDM_PERF_WINDOW          582
#define IDM_NNET_REFRESH_RATE    583
#define IDM_DESC_WINDOW          584

#define IDM_ANALYZE              600
#define IDM_ANALYZE_LOOPS        601
#define IDM_ANALYZE_ANOMALIES    602
#define IDM_CENTER_MODEL         611
							    
#define IDM_COPY_SELECTION       650
#define IDM_DELETE_SELECTION     651
#define IDM_CLEAR_BEEPERS        654
#define IDM_SELECT_ALL_BEEPERS   655

#define IDM_ALIGN_POSITIONS      671
#define IDM_ALIGN_DIRECTIONS     672
#define IDM_PACK_SHAPES          673
#define IDM_CREATE_CONNECTOR     674

#define IDX_ANIMATION_STEP       701
#define IDX_PLUG_IN_ANIMATION    703
#define IDX_MAKE_CON_STEP        710
