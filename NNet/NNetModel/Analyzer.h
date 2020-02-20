// Analyzer.h
//
// NNetModel
//
// analyze model and find suspicious structures

#pragma once

#include <vector>

class NNetModel;

using std::vector;

class ModelAnalyzer
{
public:
	static bool FindLoop( NNetModel const & );

	static void EmphasizeLoopShapes( NNetModel & );

	static void Stop( )
	{
		m_bStop = true;
	}

	static void GetEnclosingRect( MicroMeterRect & );

private:
	static bool            m_bStop;
	static int             m_iRecDepth;
	static vector<Shape *> m_shapeStack;

	static bool findLoop( Shape * const );
	static void printShapeStack( );
};
