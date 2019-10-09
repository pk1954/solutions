// Pipeline.h
//
// NNetModel

#pragma once

#include <vector>
#include "MoreTypes.h"
#include "Shape.h"
#include "Segment.h"

using std::vector;

class GraphicsInterface;
class PixelCoordsFp;
class BaseKnot;

class Pipeline : public Shape
{
public:
	Pipeline( meterPerSec const impulseSpeed )
    :	Shape( tShapeType::pipeline ),
		m_initialized ( false ),
		m_pKnotStart  ( nullptr ),
		m_pKnotEnd    ( nullptr ),
		m_width       ( 10.0_MicroMeter ),
		m_potential   ( ),
		m_impulseSpeed( impulseSpeed )
	{
	}

	void SetStartKnot( BaseKnot * );
	void SetEndKnot  ( BaseKnot * );

	void Resize( );

	void SetPulseSpeed( meterPerSec const newSpeed )
	{
		m_impulseSpeed = newSpeed;
		m_initialized = false;
		initialize();
	}

	meterPerSec GetPulseSpeed( ) const
	{
		return m_impulseSpeed;
	}

	MicroMeterPoint GetStartPoint( ) const; 
	MicroMeterPoint GetEndPoint  ( ) const; 
	MicroMeter      GetWidth     ( ) const; 

	virtual void Step( );
	virtual void Prepare( );

	virtual void Draw( GraphicsInterface &,	PixelCoordsFp const & ) const;
	virtual void MoveTo( MicroMeterPoint const & );

	virtual bool IsPointInShape( MicroMeterPoint const & ) const;
	virtual mV   GetNextOutput( ) const;

private:
	MicroMeter distance( MicroMeterPoint const &, MicroMeterPoint const & );

	void initialize( );

	bool m_initialized;
	
	BaseKnot * m_pKnotStart;
	BaseKnot * m_pKnotEnd;
	MicroMeter m_width;

	vector<mV>  m_potential;
	meterPerSec m_impulseSpeed;
};
