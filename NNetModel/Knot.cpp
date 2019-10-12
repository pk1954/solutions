// Knot.cpp 
//
// NNetModel

#include "stdafx.h"
#include "NNetModel.h"
#include "Knot.h"

void Knot::Prepare( NNetModel & model )
{
	m_mVinputBuffer = 0._mV;
	for ( auto idPipeline : m_incomming )
		m_mVinputBuffer += model.GetPipeline( idPipeline )->GetNextOutput();
	assert( m_mVinputBuffer <= PEAK_VOLTAGE );
}
