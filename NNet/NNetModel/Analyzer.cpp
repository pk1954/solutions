// Analyzer.cpp
//
// NNetModel

#include "stdafx.h"
#include <fstream>
#include "NNetModel.h"
#include "Analyzer.h"

using std::wcout;
using std::endl;

bool                  ModelAnalyzer::m_bStop;
int                   ModelAnalyzer::m_iRecDepth;
vector<Shape const *> ModelAnalyzer::m_shapeStack;

void ModelAnalyzer::printShapeStack( )
{
	for ( const auto & shape : m_shapeStack )
	{
		wcout << shape->GetId( ).GetValue() << L" - ";
	}
	wcout << endl;
}

void ModelAnalyzer::FindLoop( NNetModel const & model )
{
	int iNrOfBaseKnots { 0 };
	model.Apply2All<BaseKnot>( [&]( BaseKnot & baseKnot ) { ++iNrOfBaseKnots; return false; } );
	wcout << iNrOfBaseKnots << L" base knots found" << endl;

	for ( int i = 5; i <= 2 * iNrOfBaseKnots + 1; ++i )
	{
		int iCounter { iNrOfBaseKnots };
		m_iRecDepth = i;
		m_bStop     = false;
		wcout << L"looking for loops of size " << i << endl;
		m_shapeStack.clear();
		if ( model.Apply2All<BaseKnot>
			  (
			  	[ & ] ( BaseKnot& baseKnot )
			    {
				 	wcout << iCounter-- << L"\r";
				 	return findLoop( & baseKnot );
				}
              )
		   )
		{
			wcout << L"loop found" << endl;
			printShapeStack( );
			break;
		}
	}
}

bool ModelAnalyzer::findLoop( Shape const * const pShape )
{
	if ( m_bStop )
		return false;

	if ( m_shapeStack.size() == m_iRecDepth )
		return false;

	m_shapeStack.push_back( pShape );

	if ( 
		  ( m_shapeStack.size() > 1 ) &&                 // we are beyond the initial shape
	      ( pShape->GetId() == m_shapeStack.front()->GetId() )  // and found the same shape again
	   )
	{
		return true;  // Do not pop_back stack!
	}

	bool bLoopFound { false };

	if ( pShape->IsPipeline() )
	{
		bLoopFound = findLoop( Cast2Pipeline( pShape )->GetEndKnotPtr( ) );
	}
	else if ( pShape->IsBaseKnot() )
	{
		bLoopFound = Cast2BaseKnot( pShape )->Apply2AllOutgoingPipelinesConst( [&]( auto pipe ) { return findLoop( pipe ); } );
	}
	else
	{
		assert( false );  // shape is neither pipeline nor baseknot
	}

	if ( ! bLoopFound )
		m_shapeStack.pop_back( ); // no loop in this branch

	return bLoopFound;
}
