// Analyzer.h
//
// NNetModel
//
// analyze model and find suspicious structures

#pragma once

#include <string>
#include <vector>
#include "NNetModel.h"
#include "DisplayFunctor.h"

class Knot;
class Shape;

using std::vector;
using std::wostream;
using std::wstring;

class ModelAnalyzer
{
public:
	static void SetDisplayFunctor( DisplayFunctor * const func )
	{
		m_pDisplayFunctor = func;
	}

	static bool           FindLoop( NNetModel const & );
	static void           SelectLoopShapes( NNetModel & );
	static MicroMeterRect GetEnclosingRect( );
	static bool           FindAnomaly( NNetModel const & );
	static void           Stop( )	{ m_bStop = true; }

private:

	inline static DisplayFunctor * m_pDisplayFunctor { nullptr };
	inline static bool             m_bStop		     { false };
	inline static int              m_iRecDepth	     { 0 };
	inline static ShapeList        m_shapeStack 	 { };

	static bool findLoop( Shape * const );
	static bool hasAnomaly( Knot & );
};
