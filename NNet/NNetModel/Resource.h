// Resource.h
// NNetSimu
// Used by NNetSimu.rc

#pragma once

// identifiers below 100 are reserved for win32_utilities
// application identifiers start at 100

constexpr int IDM_FATAL_ERROR  	        { 100 };
								   
constexpr int IDC_NNET_SIMU_MAIN  	    { 102 };
constexpr int IDD_APPLY_PARAMETERS      { 105 };
constexpr int IDD_RESET_PARAMETERS      { 106 };
								   		
constexpr int IDM_SCRIPT_DIALOG         { 110 };
#define       IDM_UNDO                    115
#define       IDM_REDO                    116
constexpr int IDM_DUMP                  { 117 };
								   		
constexpr int IDI_NNETSIMU			    { 120 };
constexpr int IDI_SMALL				    { 121 };
constexpr int IDC_NNETSIMU			    { 122 };
								   		
constexpr int IDM_SLOWER                { 130 };
constexpr int IDM_FASTER                { 131 };
								   		
#define       IDM_ESCAPE                  140
constexpr int IDM_RESET                 { 141 };
#define       IDM_SELECT_ALL              142 
constexpr int IDM_SELECT_SUBTREE        { 143 };
constexpr int IDM_SELECT_NOB            { 144 };
constexpr int IDM_DESELECT_ALL          { 145 };
constexpr int IDM_DESELECT_SUBTREE      { 146 };
constexpr int IDM_DESELECT_NOB          { 147 };
constexpr int IDM_SELECTION             { 148 };
#define       IDM_DELETE                  149
								   		
#define       IDM_SAVE_MODEL              150
constexpr int IDM_ASK_AND_SAVE_MODEL    { 151 };
constexpr int IDM_SAVE_MODEL_AS         { 152 };
constexpr int IDM_OPEN_MODEL            { 153 };
constexpr int IDM_ADD_MODEL             { 154 };
constexpr int IDX_REPLACE_MODEL         { 155 };
constexpr int IDX_FILE_NOT_FOUND        { 156 };
constexpr int IDX_ERROR_IN_FILE         { 157 };
constexpr int IDM_NEW_MODEL             { 158 };
constexpr int IDM_ADD_IMPORTED_MODEL    { 159 };
								   		
constexpr int IDD_ARROWS_ON             { 161 };
constexpr int IDD_ARROWS_OFF            { 162 };
constexpr int IDD_RULER_ON              { 163 };
constexpr int IDD_RULER_OFF             { 164 };
constexpr int IDD_MEASUREMENT_ON        { 165 };
constexpr int IDD_MEASUREMENT_OFF       { 166 };
constexpr int IDD_ADD_TRACK             { 168 };
constexpr int IDD_DELETE_TRACK          { 169 };
constexpr int IDD_EMPHASIZE             { 170 };
										
constexpr int IDD_SOUND_ON              { 200 };
constexpr int IDD_SOUND_OFF             { 202 };
constexpr int IDX_PLAY_SOUND            { 203 };
								   		
constexpr int IDD_AUTO_OPEN_ON          { 210 };
constexpr int IDD_AUTO_OPEN_OFF         { 211 };
										   
constexpr int IDM_MONITOR_WINDOW        { 330 };
constexpr int IDD_ADD_EEG_SENSOR        { 345 };
constexpr int IDD_DELETE_EEG_SENSOR     { 346 };
constexpr int IDD_CONNECT               { 350 };
								   		
constexpr int IDD_INSERT_KNOT           { 460 };
constexpr int IDD_INSERT_NEURON         { 461 };
constexpr int IDD_NEW_INPUT_NEURON      { 463 };
constexpr int IDD_NEW_OUTPUT_NEURON     { 464 };
constexpr int IDD_APPEND_INPUT_NEURON   { 466 };
constexpr int IDD_APPEND_OUTPUT_NEURON  { 467 };
constexpr int IDD_DELETE_NOB            { 468 };
										
constexpr int IDD_ADD_INCOMING2KNOT     { 571 };
constexpr int IDD_ADD_INCOMING2PIPE     { 572 };
constexpr int IDD_ADD_OUTGOING2KNOT     { 573 };
constexpr int IDD_ADD_OUTGOING2PIPE     { 574 };
constexpr int IDD_TRIGGER_SOUND_DLG     { 575 };
#define       IDC_TRIGGER_SOUND_ON        576
#define       IDC_TRIGGER_SOUND_FREQ      577
#define       IDC_TRIGGER_SOUND_MSEC      578
#define       IDC_TRIGGER_SOUND_TEST      579
constexpr int IDD_STOP_ON_TRIGGER       { 580 };
								   		
#define       IDM_CRSR_WINDOW             590
#define       IDM_PARAM_WINDOW  	      591
#define       IDM_PERF_WINDOW             592
#define       IDM_DESC_WINDOW             594
constexpr int IDM_NNET_REFRESH_RATE     { 593 };

constexpr int IDM_ANALYZE               { 600 };
constexpr int IDM_ANALYZE_LOOPS         { 601 };
constexpr int IDM_ANALYZE_ANOMALIES     { 602 };
constexpr int IDM_CENTER_MODEL          { 611 };
							       		
#define       IDM_COPY_SELECTION          650 
constexpr int IDM_DELETE_SELECTION      { 651 };
constexpr int IDM_CLEAR_BEEPERS         { 654 };
constexpr int IDM_SELECT_ALL_BEEPERS    { 655 };
										
constexpr int IDM_TRIGGER_STIMULUS      { 660 };
constexpr int IDM_SIGNAL_DESIGNER       { 661 };
										
constexpr int IDM_MAKE_CONNECTOR        { 674 };
constexpr int IDD_DISC_IOCONNECTOR      { 676 };
constexpr int IDD_DISC_BASEKNOT         { 677 };
constexpr int IDD_SPLIT_NEURON          { 678 };
