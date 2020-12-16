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
	out << L"SumSignal " << m_circle;
}

void SumSignal::Draw( DrawContext const & context ) const
{
	D2D1::ColorF color1 { D2D1::ColorF::Green  };
	D2D1::ColorF color2 { D2D1::ColorF::Yellow };
	color1.a = 0.8f;
	color2.a = 0.4f;
	context.FillGradientCircle( m_circle, color1, color2 );
}

void SumSignal::Animate( bool const bOn ) const
{
	if ( bOn )
		m_pAnimationInterface->Start( m_circle );
	else
		m_pAnimationInterface->Stop( );
}

float SumSignal::GetSignalValue( ) const
{
	float fResult   { 0.0f };
	float fDsBorder { m_circle.GetRadius().GetValue() * m_circle.GetRadius().GetValue() };

	m_pMRI->Apply2All<BaseKnot>
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
