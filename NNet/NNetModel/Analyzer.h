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
class NNetModelWriterInterface;

using std::vector;
using std::wostream;
using std::wstring;

class ModelAnalyzer
{
public:
	static void SetStatusBarDisplay( DisplayFunctor * const func ) { m_pStatusBarDisplay = func; }
	static void SetEscFunc ( bool (* func )( ) ) { m_pEscFunc = func; }

	static bool FindLoop        ( NNetModelWriterInterface const & );
	static bool FindAnomaly     ( NNetModelWriterInterface const & );
	static void SelectLoopShapes( NNetModelWriterInterface & );

	static MicroMeterRect GetEnclosingRect( );

private:

	inline static DisplayFunctor * m_pStatusBarDisplay { nullptr };
	inline static bool             m_bStop		       { false };
	inline static int              m_iRecDepth	       { 0 };
	inline static vector<Shape *>  m_shapeStack        { };

	inline static bool (* m_pEscFunc )( ) { nullptr };

	static bool findLoop( Shape * const );
	static bool hasAnomaly( Knot & );

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
