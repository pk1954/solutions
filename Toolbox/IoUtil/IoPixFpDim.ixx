// IoPixFpDim.ixx
//
// Toolbox\IoUtil

module;

#include <compare>
#include <iostream>

export module IoUtil:IoPixFpDim;

import Types;
import Script;
import PixFpDimension;
import :IoPixelTypes;
import :IoConstants;

using std::wostream;

export template <typename LOG_UNIT>
PixFpDimension<LOG_UNIT> ScrReadPixFpDiemension(Script& script)
{
	PixFpDimension<LOG_UNIT> res;
	script.ScrReadSpecial(LIST_OPEN_BRACKET);
	res.SetOffset(ScrReadfPixel(script));
	script.ScrReadSpecial(SEPARATOR);
	res.SetPixelSize(ScrRead<LOG_UNIT>(script));
	script.ScrReadSpecial(LIST_CLOSE_BRACKET);
	return res;
}

export template <typename LOG_UNIT>
wostream& operator<< (wostream& out, PixFpDimension<LOG_UNIT> const& dim)
{
	out << OPEN_BRACKET << dim.m_fPixOffset
		<< SEPARATOR    << dim.m_logPixelSize
		<< CLOSE_BRACKET;
	return out;
}
