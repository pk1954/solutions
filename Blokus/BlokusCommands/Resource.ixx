// Resource.ixx
//
// BlokusCommands

export module Resource;

import Win32_Util_Resource;

// identifiers below IDM_APPL_CMD are reserved for win32_utilities
// application identifiers start at IDM_APPL_CMD

export inline int const IDD_NEXT_MOVE         { IDM_APPL_CMD + 1 };
export inline int const IDD_START_AUTO_RUN    { IDM_APPL_CMD + 2 };
export inline int const IDD_RESET             { IDM_APPL_CMD + 3 };
export inline int const IDD_START_TOURNAMENT  { IDM_APPL_CMD + 4 };
export inline int const IDD_NEXT_SHAPE        { IDM_APPL_CMD + 5 };
export inline int const IDM_TOURNAMENT_WINDOW { IDM_APPL_CMD + 6 };
export inline int const IDX_NEXT_AUTO_MOVE    { IDM_APPL_CMD + 7 };
export inline int const IDX_FINISH_MOVE       { IDM_APPL_CMD + 8 };
