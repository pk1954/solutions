// Analyzer.cpp
//
// NNetModel

#include "stdafx.h"
#include "MoreTypes.h"
#include "RectType.h"
#include "Analyzer.h"

using std::endl;
using std::to_wstring;

bool ModelAnalyzer::FindLoop( NNetModel const & model )
{
	int iNrOfShapes { model.GetNrOf<Shape>() };
	statusDisplay( to_wstring( iNrOfShapes ) + L" objects found" );

	for ( int iMaxLoopSize = 5; iMaxLoopSize <= iNrOfShapes + 1; iMaxLoopSize += 2 )
	{
		int iCounter { iMaxLoopSize };
		m_iRecDepth = iMaxLoopSize;
		m_bStop     = false;
		statusDisplay( wstring( L"Looking for loop of size " ) + to_wstring( iMaxLoopSize ) + L". Press ESC to stop." );
		m_shapeStack.clear();
		if ( model.Apply2AllB<BaseKnot>([&]( BaseKnot & baseKnot ) { return findLoop( & baseKnot ); } ) )
		{
			if ( m_bStop )  
			{
				m_shapeStack.clear();
				statusDisplay( L"analysis aborted by user" );
				return false;
			}
			else 
			{
				statusDisplay( L"loop found" );
				return true;
			}
		}
	}

	statusDisplay( L"no loop found" );
	return false;
}

// findLoop - try to find a loop in model
//
// returns true, if loop found (m_bStop == false) or aborted by user (m_bSTop == true). 
//         false, if analysis completed and no loop found

bool ModelAnalyzer::findLoop( Shape * const pShape )
{
	if ( m_bStop )
		return true;

	if ( m_shapeStack.size() == m_iRecDepth )
		return false;  // maximum search depth reached

	assert( pShape->IsDefined() );
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
		bResult = findLoop( static_cast<Pipe *>(pShape)->GetEndKnotPtr( ) );  // recursion
	}
	else if ( pShape->IsBaseKnot() )
	{
		bResult = static_cast<BaseKnot *>(pShape)->Apply2AllOutPipesB( [&]( Pipe & pipe ) { return findLoop( & pipe ); } );
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
	return ::ComputeEnclosingRect( m_shapeStack );
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
		knot.Apply2AllOutPipes( [&]( Pipe & pipe ) { m_shapeStack.push_back( & pipe ); } );
		bFoundAnomaly = true;
	}
	else if ( ! knot.HasOutgoing( ) )
	{
		knot.Apply2AllInPipes( [&]( Pipe & pipe ) { m_shapeStack.push_back( & pipe ); } );
		bFoundAnomaly = true;
	}

	if ( bFoundAnomaly )
		m_shapeStack.push_back( & knot );

	return bFoundAnomaly; 
}

bool ModelAnalyzer::FindAnomaly( NNetModel const & model )
{
	m_shapeStack.clear();
	bool const bFound { model.Apply2AllB<Knot>( [&]( Knot & knot ) { return hasAnomaly( knot ); } ) };
	if ( ! bFound )
		statusDisplay( L"no anomalies found" );
	return bFound;
}
