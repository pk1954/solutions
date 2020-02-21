// Shape.h
//
// NNetModel

#pragma once

#include "d2d1helper.h"
#include "MoreTypes.h"
#include "ShapeType.h"
#include "ShapeId.h"

class D2D_driver;
class PixelCoordsFp;
class NNetModel;

class Shape
{
public:
	Shape( ShapeType const );
	virtual ~Shape(); 

	static bool TypeFits( ShapeType const type ) { return true; }  // every shape type is a Shape

	void Emphasize( bool const bState ) { m_bEmphasized = bState; }
	bool IsEmphasized( ) { return m_bEmphasized; }

	virtual bool IsInRect      ( MicroMeterRect const & )                const = 0;
	virtual void DrawExterior  ( PixelCoordsFp &, tHighlightType const ) const = 0;
	virtual void DrawInterior  ( PixelCoordsFp & )                             = 0;
	virtual bool IsPointInShape( MicroMeterPoint const & )               const = 0;
	virtual void Prepare       ( )                                             = 0;
	virtual void Step          ( )                                             = 0;
	virtual void Recalc        ( )                                             = 0;

	virtual void Clear( ) { m_mVinputBuffer = 0.0_mV; };

	bool            IsDefined   ( ) const { return ::IsDefined( m_identifier ); }
	wchar_t const * GetName     ( ) const { return ShapeType::GetName( m_type.GetValue() ); }
	ShapeType       GetShapeType( ) const { return m_type; }
	ShapeId         GetId       ( ) const { return m_identifier; }

	bool IsPipeline   () const { return m_type.IsPipelineType   ( ); }
	bool IsKnot       () const { return m_type.IsKnotType       ( ); }
	bool IsNeuron     () const { return m_type.IsNeuronType     ( ); }
	bool IsInputNeuron() const { return m_type.IsInputNeuronType( ); }
	bool IsAnyNeuron  () const { return m_type.IsAnyNeuronType  ( ); }
	bool IsBaseKnot   () const { return m_type.IsBaseKnotType   ( ); }
	bool IsUndefined  () const { return m_type.IsUndefinedType  ( ); }

	void SetId( ShapeId const id ) { m_identifier = id;	}

	static void SetGraphics( D2D_driver const * const pGraphics ) { m_pGraphics  = pGraphics; }
	static void SetModel   ( NNetModel  const * const pModel    ) { m_pNNetModel = pModel; }

	void EnterCritSect() { EnterCriticalSection( & m_criticalSection ); }
	void LeaveCritSect() { LeaveCriticalSection( & m_criticalSection ); }

protected:

	mV m_mVinputBuffer;

	static D2D_driver const * m_pGraphics;
	static NNetModel  const * m_pNNetModel;

	D2D1::ColorF GetFrameColor( tHighlightType const ) const;
	D2D1::ColorF GetInteriorColor( mV const ) const;
	D2D1::ColorF GetInteriorColor( ) const { return GetInteriorColor( m_mVinputBuffer ); }

	float GetFillLevel( mV const ) const;
	float GetFillLevel( ) const { return GetFillLevel( m_mVinputBuffer ); };

private:

	CRITICAL_SECTION m_criticalSection; 
	bool             m_bEmphasized;
	ShapeId          m_identifier;
	ShapeType        m_type;
};
