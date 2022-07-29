// win32_PixelTypes.h
//
// Toolbox\win32_utilities

#pragma once

#include "PixelTypes.h"
#include "win32_util.h"

fPixelRect Convert2fPixelRect(RECT const & rect)
{
	return Convert2fPixelRect(Util::RECT2PixelRect(rect));
}
