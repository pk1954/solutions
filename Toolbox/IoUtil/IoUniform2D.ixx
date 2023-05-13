// IoUniform2D.ixx
//
// Toolbox\IoUtil

module;

#include <compare>
#include <iostream>

export module IoUtil:IoUniform2D;

import Uniform2D;
import :IoConstants;

using std::wostream;

export template <typename LOG_UNIT>
wostream& operator<< (wostream& out, Uniform2D<LOG_UNIT> const& uniform2D)
{
	out << OPEN_BRACKET << uniform2D.m_xDim
		<< SEPARATOR    << uniform2D.m_yDim
		<< CLOSE_BRACKET;
	return out;
}
