// Win32_PixelTypes.ixx
//
// Toolbox\win32_utilities

module;

#include "Windows.h"

export module Win32_PixelTypes;

import PixelTypes;
import Win32_PIXEL;

export fPixelRect Convert2fPixelRect(RECT const & rect)
{
	return Convert2fPixelRect(Util::RECT2PixelRect(rect));
}
