// IoPixFpDim.ixx
//
// Toolbox\IoUtil

module;

#include <compare>
#include <iostream>

export module IoUtil:IoPixFpDim;

import PixFpDimension;
import :IoConstants;

using std::wostream;

export template <typename LOG_UNIT>
wostream& operator<< (wostream& out, PixFpDimension<LOG_UNIT> const& dim)
{
	out << OPEN_BRACKET << dim.m_fPixOffset
		<< SEPARATOR    << dim.m_logPixelSize
		<< CLOSE_BRACKET;
	return out;
}
