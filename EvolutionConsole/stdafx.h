// stdafx.h : include file for standard system include files,
// or project specific include files that are used frequently, but
// are changed infrequently
//

#pragma once

#undef _HAS_VARIABLE_TEMPLATES

//lint -esym( 755, WIN32_LEAN_AND_MEAN )  globale macro not referenced
#define WIN32_LEAN_AND_MEAN             // Exclude rarely-used stuff from Windows headers
// Windows Header Files:
#include <windows.h>

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>



// reference additional headers your program requires here
