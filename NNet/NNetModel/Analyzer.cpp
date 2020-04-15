// Analyzer.cpp
//
// NNetModel

#include "stdafx.h"
#include "MoreTypes.h"
#include "RectType.h"
#include "NNetModel.h"
#include "Analyzer.h"

using std::endl;
using std::to_wstring;

bool ModelAnalyzer::FindLoop( NNetModel const & model )
{
	int iNrOfShapes { model.GetNrOfShapes() };
	(* m_pDisplayFunctor)( to_wstring( iNrOfShapes ) + L" objects found" );

	for ( int iMaxLoopSize = 5; iMaxLoopSize <= iNrOfShapes + 1; iMaxLoopSize += 2 )
	{
		int iCounter { iMaxLoopSize };
		m_iRecDepth = iMaxLoopSize;
		m_bStop     = false;
		(* m_pDisplayFunctor)( wstring( L"Looking for loop of size " ) + to_wstring( iMaxLoopSize ) + L". Press ESC to stop." );
		m_shapeStack.clear();
		if ( model.Apply2AllB<BaseKnot>([&]( BaseKnot & baseKnot ) { return findLoop( & baseKnot ); } ) )
		{
			(* m_pDisplayFunctor)( wstring( m_bStop ? L"analysis aborted by user" : L"loop found" ) );
			return ! m_bStop;
		}
	}

	(* m_pDisplayFunctor)( wstring( L"no loop found" ) );
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
	else if ( pShape->IsPipe() )
	{
		Pipe * pPipe { Cast2Pipe( pShape ) };
		bResult = findLoop( pPipe->GetEndKnotPtr( ) );  // recursion
	}
	else if ( pShape->IsBaseKnot() )
	{
		BaseKnot * pBaseKnot { Cast2BaseKnot( pShape ) };
		bResult = pBaseKnot->Apply2AllOutPipesB( [&]( auto pipe ) { return findLoop( pipe ); } );
	}
	else
	{
		assert( false );  // shape is neither Pipe nor baseknot
	}

	if ( ! bResult )
		m_shapeStack.pop_back( ); // no loop in this branch

	return bResult;
}

MicroMeterRect ModelAnalyzer::GetEnclosingRect( )
{
	return ::GetEnclosingRect( m_shapeStack );
}

void ModelAnalyzer::SelectLoopShapes( NNetModel & model )
{
	for ( const auto & pShape : m_shapeStack )
		pShape->Select( tBoolOp::opTrue );
}

bool ModelAnalyzer::hasAnomaly( Knot & knot )
{
	bool bFoundAnomaly { false };

	if ( ! knot.HasIncoming( ) )
	{
		knot.Apply2AllOutPipes( [&]( auto pipe ) { m_shapeStack.push_back( pipe ); } );
		bFoundAnomaly = true;
	}
	else if ( ! knot.HasOutgoing( ) )
	{
		knot.Apply2AllInPipes( [&]( auto pipe ) { m_shapeStack.push_back( pipe ); } );
		bFoundAnomaly = true;
	}

	if ( bFoundAnomaly )
		m_shapeStack.push_back( & knot );

	return bFoundAnomaly; 
}

bool ModelAnalyzer::FindAnomaly( NNetModel const & model )
{
	m_shapeStack.clear();
	return model.Apply2AllB<Knot>( [&]( Knot & knot ) { return hasAnomaly( knot ); } );
}
