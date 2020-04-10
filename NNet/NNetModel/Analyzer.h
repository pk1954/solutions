// Analyzer.h
//
// NNetModel
//
// analyze model and find suspicious structures

#pragma once

#include <string>
#include <vector>
#include "DisplayFunctor.h"

class NNetModel;
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
	static void           EmphasizeLoopShapes( NNetModel & );
	static MicroMeterRect GetEnclosingRect( );
	static bool           FindAnomaly( NNetModel const & );
	static void           Stop( )	{ m_bStop = true; }

private:

	static DisplayFunctor * m_pDisplayFunctor;
	static bool             m_bStop;
	static int              m_iRecDepth;
	static vector<Shape *>  m_shapeStack;

	static bool findLoop( Shape * const );
	static bool hasAnomaly( Knot & );
};
