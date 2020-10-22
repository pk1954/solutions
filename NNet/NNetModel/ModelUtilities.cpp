// ModelUtilities.cpp 
//
// NNetModel

#include "stdafx.h"
#include "ModelUtilities.h"

void Expand( MicroMeterRect & rect, Shape const * pShape )
{
	if ( pShape )
	{
		if ( pShape->IsBaseKnot() )
		{
			rect.Expand( Cast2BaseKnot( pShape )->GetPosition() );
		}
		else if ( pShape->IsPipe() )
		{
			rect.Expand( Cast2Pipe( pShape )->GetStartPoint() );
			rect.Expand( Cast2Pipe( pShape )->GetEndPoint() );
		}
		else
		{
			assert( false );
		}
	}
}
