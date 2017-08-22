// stdafx.h: Includedatei für Standardsystem-Includedateien
// oder häufig verwendete projektspezifische Includedateien,
// die nur in unregelmäßigen Abständen geändert werden.
//

#pragma once

#include "targetver.h"

//lint -esym( 755, WIN32_LEAN_AND_MEAN )  globale macro not referenced
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#include <stdlib.h>
#include <memory.h>
#include <tchar.h>
#include <assert.h>
