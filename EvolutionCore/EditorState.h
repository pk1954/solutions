// EditorState.h
//

#pragma once

#include "EvolutionTypes.h"
#include "BoolOp.h"
#include "Manipulator.h"
#include "gridBrush.h"
#include "gridRect.h"

class Grid;

class EditorState
{
public:

    EditorState( Grid * const pGrid  )
		: m_brush( pGrid )
	{ 
		Reset( );
	}

    virtual ~EditorState( ) { }

	void Reset( )
	{
		m_brush.Reset( );
		m_bSimulationMode = false;
	}

	void EditorDoEdit( GridPoint const gpCenter )
	{
		(m_brush)( gpCenter ); 
	}

	void SetBrushShape    ( tShape     const s ) { m_brush.SetShape    ( s ); }
    void SetBrushIntensity( short      const s ) { m_brush.SetIntensity( s ); }
    void SetBrushRadius   ( GRID_COORD const r ) { m_brush.SetRadius   ( r ); }
	void SetBrushOperator ( tOperator  const o ) { m_brush.SetOperator ( o ); }
  	void SetBrushMode     ( tBrushMode const m ) { m_brush.SetBrushMode( m ); }
    void SetSimulationMode( tBoolOp    const o ) { ApplyOp( m_bSimulationMode, o ); }

    tOperator  GetBrushOperator ( ) const { return m_brush.GetOperator(); }
	short      GetBrushIntensity( ) const { return m_brush.GetIntensity(); }
    tShape     GetBrushShape( )     const { return m_brush.GetShape(); }
    GRID_COORD GetBrushSize( )      const { return m_brush.GetRadius(); }
    tBrushMode GetBrushMode( )      const { return m_brush.GetBrushMode(); }
    bool       GetSimulationMode( ) const { return m_bSimulationMode; }

    bool operator!=( EditorState const & other ) const
    { 
        return ( m_brush != other.m_brush ) || ( m_bSimulationMode != other.m_bSimulationMode );
    }
 
private:
	bool      m_bSimulationMode;
	GridBrush m_brush;
};
