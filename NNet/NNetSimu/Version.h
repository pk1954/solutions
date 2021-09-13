#pragma once

#include "winver.h"

#define STRINGIZE(s) #s

#define VERSION_MAJOR               1
#define VERSION_MINOR               0
#define VERSION_REVISION            0
#define VERSION_BUILD               0

#ifdef _DEBUG
	#define VER_DEBUG_RELEASE   "Debug"
#else
	#define VER_DEBUG_RELEASE   "Release"
#endif

#if _WIN64
	#define VER_ARCHITECTURE   "x64"
#else
	#define VER_ARCHITECTURE   "x86"
#endif

#define VER_FILE_DESCRIPTION_STR    VER_DEBUG_RELEASE " "  VER_ARCHITECTURE
#define VER_FILE_VERSION            VERSION_MAJOR, VERSION_MINOR, VERSION_REVISION, VERSION_BUILD
#define VER_FILE_VERSION_STR        VER_DEBUG_RELEASE " "  VER_ARCHITECTURE " "   

#define VER_PRODUCTNAME_STR         "NNetSimu 2.1"
#define VER_PRODUCT_VERSION         VER_FILE_VERSION
#define VER_PRODUCT_VERSION_STR     VER_FILE_VERSION_STR
#define VER_ORIGINAL_FILENAME_STR   VER_PRODUCTNAME_STR ".exe"
#define VER_INTERNAL_NAME_STR       VER_ORIGINAL_FILENAME_STR
#define VER_COPYRIGHT_STR           "Copyright (C) 2021"
