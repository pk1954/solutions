// SumSignal.cpp
// 
// NNetWindows

#include "stdafx.h"
#include "DrawContext.h"
#include "NNetColors.h"
#include "NNetModelReaderInterface.h"
#include "SumSignal.h"

void SumSignal::WriteSignalData( wostream & out ) const
{
	out << L" sum " << m_circle;
}

void SumSignal::Draw( DrawContext const & context ) const
{
	context.FillCircle( m_circle, NNetColors::SELECTION_RECT );
	context.DrawCircle
	( 
		m_circle * (1.0f - BORDER / 2.0f), 
		NNetColors::SELECTION_RECT, 
		m_circle.GetRadius() * (1.0f - BORDER) 
	);
}

void SumSignal::Animate( bool const ) const
{

}

float SumSignal::GetSignalValue( ) const
{
	float fResult   { 0.0f };
	float fDsBorder { m_circle.GetRadius().GetValue() * m_circle.GetRadius().GetValue() };

	m_pModelReaderInterface->Apply2All<BaseKnot>
	( 		
		[&](BaseKnot const & b) 
		{  
			float fDsBaseKnot { DistSquare( b.GetPosition(), m_circle.GetPosition() ) };
			if ( fDsBaseKnot < fDsBorder )  // is b in circle?
			{
				mV    voltage { b.GetVoltage() };
				float fFactor { 1.0f - fDsBaseKnot / fDsBorder };
				fResult += voltage.GetValue() * fFactor;
			}
		} 
	);

	return fResult;
}
