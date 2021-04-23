// RotationAnimation.cpp
//
// NNetWindows

#include "stdafx.h"
#include "Resource.h"
#include "ShapeIdList.h"
#include "NNetModelWriterInterface.h"
#include "NNetModelCommands.h"
#include "CalcOrthoVector.h"
#include "RotationAnimation.h"

void RotationAnimation::Initialize
( 
	HWND               const   hwnd,
	NNetModelWriterInterface & nmwi,
	NNetModelCommands        & commands
)
{
	//m_pNMWI           = & nmwi;
	//m_pModelCommands  = & commands;
	//m_upRotAnimation  = make_unique<RotAnimation>(IDX_ROTATION_ANIMATION, hwnd); 
}

bool const RotationAnimation::prepareData()
{
	//ShapeType const shapeType { m_pNMWI->GetUPShapes().DetermineShapeType() };

	//if ( shapeType.IsUndefinedType() )
	//{
	//	m_line = MicroMeterLine::NULL_VAL();
	//	return false;
	//}

	//ALIGN_VECTOR points;

	//m_pNMWI->GetUPShapes().Apply2AllSelected
	//( 
	//	shapeType,
	//	[&points](Shape & s) { points.push_back( ALIGN_PNT { static_cast<ConnNeuron *>(&s) } ); } 
	//);

	//if ( ! calcMaxDistLine( points ) )
	//	return false;

	//calcDistances( points );
	//sortDistances( points );

	//m_shapesAnimated.Clear();
	//for ( auto & it : points )
	//	m_shapesAnimated.Add( it.pConnectionNeuron );

	return true;
}

bool const RotationAnimation::RotateSelection( Connector const & connector2animate )
{
	if ( ! prepareData() )
		return false;

	// fill animation vectors

	//MicroMeterPosDir      posDirTarget { umPntTargetStart, Vector2Radian(m_orthoVector) };
	//MicroMeterPointVector umPntVectorTarget {};

	//m_shapesAnimated.Apply2All
	//(	
	//	[&](ConnNeuron const & c)	
	//	{ 
	//		MicroMeterPosDir posDir { c.GetPos(), posDirTarget.GetDir() };
	//		umPntVectorTarget.Add( posDir );
	//		posDirTarget += umPntSingleVector;
	//	}	
	//);

	//MicroMeterPointVector umPntVectorStart( m_shapesAnimated );
	//MicroMeterPointVector umPntVectorDiff { umPntVectorTarget - umPntVectorStart };

	//Radian radDiffMax;
	//umPntVectorDiff.Apply2All
	//(
	//	[&](MicroMeterPosDir const & elem)
	//	{
	//		if (elem.GetDir() > radDiffMax)
	//			radDiffMax = elem.GetDir();
	//	}
	//);
	//Radian     const radPerStep { Degrees2Radian(6.0_Degrees) };
	//MicroMeter const umPerStep  { NEURON_RADIUS / 5.0f };
	//float      const fSteps     { radDiffMax / radPerStep };

	//m_upRotAnimation->SetNrOfSteps( Cast2UnsignedInt(fSteps) + 1 );
	//m_upRotAnimation->Start( umPntVectorStart, umPntVectorTarget);

	return true;
}

void RotationAnimation::AnimationStep()
{
	//unique_ptr<ShapeIdList> upShapeIds { make_unique<ShapeIdList>(m_shapesAnimated) };

	//m_pModelCommands->SetConnectionNeurons
	//(
	//	m_upRotAnimation->GetActual(), 
	//	move(upShapeIds)
	//);
}
