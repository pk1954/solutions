// Analyzer.h
//
// NNetModel
//
// analyze model and find suspicious structures

#pragma once

#include <vector>

using std::vector;

class NNetModel;
class Shape;

class ModelAnalyzer
{
public:
	static void FindLoop( NNetModel const & );

	static void Stop( )
	{
		m_bStop = true;
	}

private:
	static bool                  m_bStop;
	static int                   m_iRecDepth;
	static vector<Shape const *> m_shapeStack;

	static bool findLoop( Shape const * const );
	static void printShapeStack( );
};
