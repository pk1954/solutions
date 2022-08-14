// Win32_PixelTypes.ixx
//
// Toolbox\win32_utilities

module;

#include "Win32_PIXEL.h"

export module Win32_PixelTypes;

import PixelTypes;

export fPixelRect Convert2fPixelRect(RECT const & rect)
{
	return Convert2fPixelRect(Util::RECT2PixelRect(rect));
}
