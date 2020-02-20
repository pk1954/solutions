// Analyzer.cpp
//
// NNetModel

#include "stdafx.h"
#include <fstream>
#include "MoreTypes.h"
#include "RectType.h"
#include "NNetModel.h"
#include "Analyzer.h"

using std::wcout;
using std::endl;

bool            ModelAnalyzer::m_bStop;
int             ModelAnalyzer::m_iRecDepth;
vector<Shape *> ModelAnalyzer::m_shapeStack;

void ModelAnalyzer::printShapeStack( )
{
	for ( const auto & shape : m_shapeStack )
	{
		wcout << shape->GetId( ).GetValue() << L" - ";
	}
	wcout << endl;
}

void ModelAnalyzer::GetEnclosingRect( MicroMeterRect & rect )
{
	rect = MicroMeterRect::ZERO_VAL();
	for ( const auto & pShape : m_shapeStack )
	{
		if ( pShape->IsPipeline() )
		{
			rect.Expand( Cast2Pipeline( pShape )->GetStartPoint( ) );
			rect.Expand( Cast2Pipeline( pShape )->GetEndPoint( ) );
		}
		else 
		{
			rect.Expand( Cast2BaseKnot( pShape )->GetPosition() );
		}
	}
}

bool ModelAnalyzer::FindLoop( NNetModel const & model )
{
	int iNrOfShapes { model.GetNrOfShapes() };
	wcout << iNrOfShapes << L" objects found" << endl;

	for ( int iMaxLoopSize = 5; iMaxLoopSize <= iNrOfShapes + 1; iMaxLoopSize += 2 )
	{
		int iCounter { iMaxLoopSize };
		m_iRecDepth = iMaxLoopSize;
		m_bStop     = false;
		wcout << L"looking for loops of size " << iMaxLoopSize << endl;
		m_shapeStack.clear();
		if ( model.Apply2All<BaseKnot>
			  (
			  	[ & ] ( BaseKnot & baseKnot )
			    {
				 	wcout << iCounter-- << L"\r";
				 	return findLoop( & baseKnot );
				}
              )
		   )
		{
			wcout << L"loop found" << endl;
			printShapeStack( );
			return true;
		}
	}

	return false;
}

bool ModelAnalyzer::findLoop( Shape * const pShape )
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
		bLoopFound = Cast2BaseKnot( pShape )->Apply2AllOutgoingPipelines( [&]( auto pipe ) { return findLoop( pipe ); } );
	}
	else
	{
		assert( false );  // shape is neither pipeline nor baseknot
	}

	if ( ! bLoopFound )
		m_shapeStack.pop_back( ); // no loop in this branch

	return bLoopFound;
}

void ModelAnalyzer::EmphasizeLoopShapes( NNetModel & model )
{
	for ( const auto & pShape : m_shapeStack )
		pShape->Emphasize( true );
	model.SetEmphasizeMode( true );
}
