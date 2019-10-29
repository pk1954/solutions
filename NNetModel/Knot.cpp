// Knot.cpp 
//
// NNetModel

#include "stdafx.h"
#include "NNetModel.h"
#include "Knot.h"

void Knot::Prepare( )
{
	m_mVinputBuffer = 0._mV;
	for ( auto idPipeline : m_incoming )
		m_mVinputBuffer += m_pModel->GetPipeline( idPipeline )->GetNextOutput();
	assert( m_mVinputBuffer <= PEAK_VOLTAGE );
}
