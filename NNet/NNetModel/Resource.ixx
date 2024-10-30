// Resource.ixx
//
// NNetModel

export module Resource;

// identifiers below 100 are reserved for win32_utilities
// application identifiers start at 100

export inline int const IDM_FATAL_ERROR  	          { 100 };
								   	         		  
export inline int const IDC_NNET_SIMU_MAIN  	      { 102 };
export inline int const IDD_APPLY                     { 105 };
export inline int const IDD_RESET                     { 107 };
										    		  
export inline int const IDM_SCRIPT_DIALOG             { 110 };
export inline int const IDM_DUMP                      { 117 };
								   	         		  
export inline int const IDM_ESCAPE                    { 140 };
export inline int const IDM_RESET_DYNAMIC_DATA        { 141 };
								   	         		  
export inline int const IDX_EXPORT_FINISHED           { 161 };
export inline int const IDX_REPLACE_MODEL             { 162 };
export inline int const IDX_ASK_REPLACE_MODEL         { 163 };
													  
export inline int const IDM_SAVE_MODEL                { 170 };
export inline int const IDM_ASK_AND_SAVE_MODEL        { 171 };
export inline int const IDM_SAVE_MODEL_AS             { 172 };
export inline int const IDM_OPEN_MODEL                { 173 };
export inline int const IDM_ADD_MODULE                { 174 };
export inline int const IDM_NEW_MODEL                 { 175 };
export inline int const IDM_IMPORT_MODEL              { 176 };
export inline int const IDM_ADD_IMPORTED_MODEL        { 177 };
export inline int const IDM_RELOAD_MODEL              { 178 };
													  
export inline int const IDM_SAVE_SCAN                 { 180 };
													  
export inline int const IDM_SCAN                      { 190 };
export inline int const IDM_SCAN_STEP                 { 191 };
export inline int const IDM_STARTING_SCAN             { 192 };
export inline int const IDM_FINISHING_SCAN            { 193 };
export inline int const IDM_UNLOCK                    { 194 };
													  
export inline int const IDD_ARROW_ANIMATION           { 270 };
													  
export inline int const IDD_MEASUREMENT_ON            { 285 };
export inline int const IDD_MEASUREMENT_OFF           { 286 };
export inline int const IDD_ADD_TRACK                 { 287 };
export inline int const IDD_DELETE_TRACK              { 288 };
export inline int const IDD_EMPHASIZE                 { 289 };
export inline int const IDD_DELETE_EMPTY_TRACKS       { 292 };
													  
export inline int const IDD_ARROWS                    { 310 };
export inline int const IDD_AUTO_OPEN                 { 312 };
export inline int const IDD_SENSOR_PNTS               { 313 };
export inline int const IDD_PERF_MON_MODE             { 314 };
export inline int const IDD_SCAN_AREA                 { 315 };
export inline int const IDD_MODEL_FRONT               { 316 };
export inline int const IDD_ASK_NOT_UNDOABLE          { 317 };
													  
export inline int const IDM_INPUT_CABLES_ALL          { 340 };
export inline int const IDM_INPUT_CABLES_NONSTD       { 341 };
export inline int const IDM_INPUT_CABLES_ACTIVE       { 342 };
export inline int const IDM_INPUT_CABLES_NONE         { 343 };
													  
export inline int const IDM_MONITOR_WINDOW            { 430 };
export inline int const IDD_SIGNAL_TOO_HIGH           { 440 };
export inline int const IDD_ADD_MICRO_SENSOR          { 441 };
export inline int const IDD_DEL_MICRO_SENSOR          { 442 };
export inline int const IDD_ADD_EEG_SENSOR            { 445 };
export inline int const IDD_DELETE_EEG_SENSOR         { 446 };
export inline int const IDD_DELETE_SIGNAL             { 447 };
export inline int const IDD_SCALE_EEG_SIGNALS         { 448 };
export inline int const IDD_CONNECT                   { 450 };
								   	         		  
export inline int const IDD_INSERT_KNOT               { 460 };
export inline int const IDD_INSERT_NEURON             { 461 };
export inline int const IDD_NEW_IO_LINE_PAIR          { 463 };
export inline int const IDD_DELETE_NOB                { 468 };
export inline int const IDD_DETACH_NOB                { 469 };
													  
export inline int const IDD_EXTEND_INPUTLINE          { 570 };
export inline int const IDD_EXTEND_OUTPUTLINE         { 571 };
export inline int const IDD_ADD_INCOMING2NEURON       { 572 };
export inline int const IDD_CREATE_SYNAPSE            { 573 };
export inline int const IDD_CREATE_FORK               { 575 };
export inline int const IDD_STOP_ON_TRIGGER           { 580 };
								   		     		  
export inline int const IDM_CRSR_WINDOW               { 590 };
export inline int const IDM_PARAM_WINDOW  	          { 591 };
export inline int const IDM_PERF_WINDOW               { 592 };
export inline int const IDM_DESC_WINDOW               { 593 };
export inline int const IDM_SIG_DESIGNER              { 594 };
export inline int const IDM_LUT_DESIGNER              { 595 };
export inline int const IDM_VIEWER_WINDOW             { 596 };
									        		  
export inline int const IDM_CENTER_MODEL              { 611 };
							       		     		  
export inline int const IDM_COPY_SELECTION            { 650  };
export inline int const IDM_DELETE_SELECTION          { 651 };
										     		  
export inline int const IDM_TRIGGER_STIMULUS          { 660 };
export inline int const IDM_SIGNAL_DESIGNER_LAYOUT    { 661 };
export inline int const IDM_SIGNAL_DESIGNER_TOGGLE    { 662 };
export inline int const IDM_SIGNAL_DESIGNER_PREVIEW   { 664 };
													  
export inline int const IDD_DISC_IOCONNECTOR          { 676 };
export inline int const IDD_SPLIT_NEURON              { 678 };
													  
export inline int const IDD_STD_SIGNAL_GENERATOR      { 700 };
export inline int const IDD_SELECT_SIGNAL_GENERATOR   { 701 };
export inline int const IDD_NEW_SIGNAL_GENERATOR      { 702 };
export inline int const IDD_DELETE_SIGNAL_GENERATOR   { 703 };
export inline int const IDD_RENAME_SIGNAL_GENERATOR   { 704 };
export inline int const IDD_REGISTER_SIG_GEN          { 706 };
export inline int const IDD_TRIGGER_SIGNAL_DESIGNER   { 707 };
export inline int const IDD_ATTACH_SIG_GEN_TO_LINE    { 708 };
export inline int const IDD_ATTACH_SIG_GEN_TO_CONN    { 709 };
export inline int const IDD_ADD_SIG_GEN_TO_MONITOR    { 711 };
													  
export inline int const IDD_FILTER                    { 800 };
													  
export inline int const IDD_REMOVE_SCAN_PANEL         { 900 };
export inline int const IDD_ANIMATE_PANELS            { 901 };
