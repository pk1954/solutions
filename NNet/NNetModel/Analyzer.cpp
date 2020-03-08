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

bool ModelAnalyzer::FindLoop( NNetModel const & model )
{
	int iNrOfShapes { model.GetNrOfShapes() };
	wcout << iNrOfShapes << L" objects found" << endl;

	for ( int iMaxLoopSize = 5; iMaxLoopSize <= iNrOfShapes + 1; iMaxLoopSize += 2 )
	{
		int iCounter { iMaxLoopSize };
		m_iRecDepth = iMaxLoopSize;
		m_bStop     = false;
		wcout << L"looking for loops of size " << iMaxLoopSize << L". Press ESC to stop." << endl;
		m_shapeStack.clear();
		if ( model.Apply2All<BaseKnot>
			  (
			  	[ & ] ( BaseKnot & baseKnot )
			    {
				 	wcout << iCounter-- << L"\r";
					if ( m_bStop )
						return true;
					else 
				 		return findLoop( & baseKnot );
				}
              )
		   )
		{
			if ( m_bStop )
			{
				wcout << L"analysis aborted by user" << endl;
				return false;
			}
			else
			{
				wcout << L"loop found" << endl;
				printShapeStack( );
				return true;
			}
		}
	}

	wcout << L"no loop found" << endl;
	return false;
}

// findLoop - try to find a loop in model
//
// returns true, if loop found or aborted by user
//         false, if analysis completed and no loop found

bool ModelAnalyzer::findLoop( Shape * const pShape )
{
	if ( m_bStop )
		return true;

	if ( m_shapeStack.size() == m_iRecDepth )
		return false;  // maximum search depth reached

	m_shapeStack.push_back( pShape );

	bool bResult { false };

	if ( 
		  ( m_shapeStack.size() > 1 ) &&                        // we are beyond the initial shape
	      ( pShape->GetId() == m_shapeStack.front()->GetId() )  // and found the same shape again
	   )
	{
		bResult = true;  // loop found. Do not pop_back stack!
	}
	else if ( pShape->IsPipeline() )
	{
		Pipeline * pPipe { Cast2Pipeline( pShape ) };
		bResult = findLoop( pPipe->GetEndKnotPtr( ) );
	}
	else if ( pShape->IsBaseKnot() )
	{
		BaseKnot * pBaseKnot { Cast2BaseKnot( pShape ) };
		bResult = pBaseKnot->Apply2AllOutgoingPipelines( [&]( auto pipe ) { return findLoop( pipe ); } );
	}
	else
	{
		assert( false );  // shape is neither pipeline nor baseknot
	}

	if ( ! bResult )
		m_shapeStack.pop_back( ); // no loop in this branch

	return bResult;
}

MicroMeterRect ModelAnalyzer::GetEnclosingRect( )
{
	return ::GetEnclosingRect( m_shapeStack );
}

void ModelAnalyzer::EmphasizeLoopShapes( NNetModel & model )
{
	for ( const auto & pShape : m_shapeStack )
		pShape->Emphasize( true );
	model.SetEmphasizeMode( true );
}
