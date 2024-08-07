// Win32_PixelTypes.ixx
//
// Toolbox\win32_utilities

module;

#include <Windows.h>

export module Win32_PixelTypes;

import Types;
import WinBasics;

export fPixelRect Convert2fPixelRect(RECT const & rect)
{
	return Convert2fPixelRect(RECT2PixelRect(rect));
}
