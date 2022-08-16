// Resource.h
// NNetSimu
// Used by NNetSimu.rc

#pragma once

// identifiers below 100 are reserved for win32_utilities
// application identifiers start at 100

#define       IDM_FATAL_ERROR  	             100
								   	         
#define       IDC_NNET_SIMU_MAIN  	         102
#define       IDD_APPLY                      105
#define       IDD_RESET                      107
										    
#define       IDM_SCRIPT_DIALOG              110
#define       IDM_UNDO                       115
#define       IDM_REDO                       116
#define       IDM_DUMP                       117
								   	         
#define       IDI_NNETSIMU			         120
#define       IDI_SMALL				         121
#define       IDC_NNETSIMU			         122
								   	         
#define       IDM_SLOWER                     130
#define       IDM_FASTER                     131
								   	         
#define       IDM_ESCAPE                     140
#define       IDM_RESET_DYNAMIC_DATA         141
#define       IDM_SELECT_ALL                 142 
#define       IDM_SELECT_SUBTREE             143
#define       IDM_SELECT_NOB                 144
#define       IDM_DESELECT_ALL               145
#define       IDM_DESELECT_SUBTREE           146
#define       IDM_DESELECT_NOB               147
#define       IDM_SELECTION                  148
#define       IDM_DELETE                     149
								   	         
#define       IDM_SAVE_MODEL                 150
#define       IDM_ASK_AND_SAVE_MODEL         151
#define       IDM_SAVE_MODEL_AS              152
#define       IDM_OPEN_MODEL                 153
#define       IDM_ADD_MODEL                  154
#define       IDM_NEW_MODEL                  155
#define       IDM_ADD_IMPORTED_MODEL         156

#define       IDX_READ_PROGRESS_REPORT       160
#define       IDX_WRITE_PROGRESS_REPORT      161
#define       IDX_REPLACE_MODEL              162
#define       IDX_FILE_NOT_FOUND             163
#define       IDX_ERROR_IN_FILE              164
							   	         
#define       IDD_ARROWS                     180
#define       IDD_ARROWS_ON                  181
#define       IDD_ARROWS_OFF                 182
#define       IDD_RULER_ON                   183
#define       IDD_RULER_OFF                  184
#define       IDD_MEASUREMENT_ON             185
#define       IDD_MEASUREMENT_OFF            186
#define       IDD_ADD_TRACK                  187
#define       IDD_DELETE_TRACK               188
#define       IDD_EMPHASIZE                  189
#define       IDD_PERF_MON_MODE_ON           190
#define       IDD_PERF_MON_MODE_OFF          191
#define       IDD_DELETE_EMPTY_TRACKS        192

#define       IDD_SOUND_ON                   200
#define       IDD_SOUND_OFF                  202
#define       IDX_PLAY_SOUND                 203
								   	         
#define       IDD_AUTO_OPEN_ON               210
#define       IDD_AUTO_OPEN_OFF              211
									         
#define       IDD_SENSOR_PNTS                220
#define       IDD_SENSOR_PNTS_ON             221
#define       IDD_SENSOR_PNTS_OFF            222
									         
#define       IDM_MONITOR_WINDOW             330
#define       IDD_SIGNAL_TOO_HIGH            340
#define       IDD_ADD_EEG_SENSOR             345
#define       IDD_DELETE_EEG_SENSOR          346
#define       IDD_DELETE_SIGNAL              347
#define       IDD_SCALE_EEG_SIGNALS          348
#define       IDD_CONNECT                    350
								   	         
#define       IDD_INSERT_KNOT                460
#define       IDD_INSERT_NEURON              461
#define       IDD_NEW_IO_LINE_PAIR           463
#define       IDD_DELETE_NOB                 468
										    
#define       IDD_ADD_INCOMING2BASEKNOT      571
#define       IDD_ADD_INCOMING2PIPE          572
#define       IDD_ADD_OUTGOING2BASEKNOT      573
#define       IDD_ADD_OUTGOING2PIPE          574
#define       IDD_STOP_ON_TRIGGER            580
								   		     
#define       IDM_CRSR_WINDOW                590
#define       IDM_PARAM_WINDOW  	         591
#define       IDM_PERF_WINDOW                592
#define       IDM_DESC_WINDOW                593
#define       IDM_SIG_DESIGNER               594
#define       IDM_NNET_REFRESH_RATE          595
									        
#define       IDM_ANALYZE                    600
#define       IDM_ANALYZE_LOOPS              601
#define       IDM_ANALYZE_ANOMALIES          602
#define       IDM_CENTER_MODEL               611
							       		     
#define       IDM_COPY_SELECTION             650 
#define       IDM_DELETE_SELECTION           651
										     
#define       IDM_TRIGGER_STIMULUS           660
#define       IDM_SIGNAL_DESIGNER_INTEGRATED 662
#define       IDM_SIGNAL_DESIGNER_STACKED    663

#define       IDM_MAKE_CONNECTOR             674
#define       IDD_DISC_IOCONNECTOR           676
#define       IDD_SPLIT_NEURON               678

#define       IDD_STD_SIGNAL_GENERATOR       700
#define       IDD_SELECT_SIGNAL_GENERATOR    701
#define       IDD_NEW_SIGNAL_GENERATOR       702
#define       IDD_DELETE_SIGNAL_GENERATOR    703
#define       IDD_RENAME_SIGNAL_GENERATOR    704
#define       IDD_SELECT_SIG_GEN_CLIENTS     705
#define       IDD_REGISTER_SIG_GEN           706
#define       IDD_TRIGGER_SIGNAL_DESIGNER    707
#define       IDD_ATTACH_SIG_GEN_TO_LINE     708
#define       IDD_ATTACH_SIG_GEN_TO_CONN     709
#define       IDD_ATTACH_SIG_GEN_TO_SEL      710
#define       IDD_ADD_SIG_GEN_TO_MONITOR     711
