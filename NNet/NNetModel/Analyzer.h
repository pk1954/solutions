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
class NNetModelReadInterface;
class NNetModelWriterInterface;

using std::vector;
using std::wostream;
using std::wstring;

using ShapeStack = vector<Shape const *>;

class ModelAnalyzer
{
public:
	static void SetStatusBarDisplay( DisplayFunctor * const func ) { m_pStatusBarDisplay = func; }
	static void SetEscFunc ( bool (* func )( ) ) { m_pEscFunc = func; }

	static ShapeStack const FindLoop   ( NNetModelReaderInterface const & );
	static ShapeStack const FindAnomaly( NNetModelReaderInterface const & );

	static MicroMeterRect GetEnclosingRect( );

private:

	inline static DisplayFunctor * m_pStatusBarDisplay { nullptr };
	inline static bool             m_bStop		       { false };
	inline static int              m_iRecDepth	       { 0 };
	inline static ShapeStack       m_shapeStack        { };

	inline static bool (* m_pEscFunc )( ) { nullptr };

	static bool findLoop( Shape const & );
	static bool hasAnomaly( Knot const & );

	static void statusDisplay( wstring const str ) 
	{ 
		if ( m_pStatusBarDisplay )
			(* m_pStatusBarDisplay)( str );
	}

	static void statusDisplay( wchar_t const * pStr )
	{
		statusDisplay( wstring( pStr ) );
	}
};
