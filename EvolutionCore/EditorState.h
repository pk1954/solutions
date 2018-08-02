// EditorState.h
//

#pragma once

#include "EvolutionTypes.h"
#include "BoolOp.h"

class Grid;

class EditorState
{
public:

    EditorState( );
    virtual ~EditorState( ) { }

	void Reset( );

    void EditorDoEdit( Grid &, GridPoint const );

	void SetBrushStrategy ( tBrushMode     const mode  ) { m_brushMode      = mode;  }
    void SetBrushShape    ( tShape         const shape ) { m_brushShape     = shape; }
    void SetBrushSize     ( GRID_COORD     const size  ) { m_brushSize      = size;  }
    void SetBrushIntensity( unsigned short const usInt ) { m_brushIntensity = usInt; }
    void SetSimulationMode( tBoolOp        const op    ) { ApplyOp( m_bSimulationMode, op ); }
  
    unsigned short GetBrushIntensity( ) const { return m_brushIntensity; }
    tShape         GetBrushShape( )     const { return m_brushShape; }
    GRID_COORD     GetBrushSize( )      const { return m_brushSize; }
    tBrushMode     GetBrushMode( )      const { return m_brushMode; }
    bool           GetSimulationMode( ) const { return m_bSimulationMode; }

    bool operator!=( EditorState const & other ) const
    {
        return ( m_brushMode       != other.m_brushMode )
            || ( m_brushSize       != other.m_brushSize )      
            || ( m_brushShape      != other.m_brushShape )
            || ( m_brushIntensity  != other.m_brushIntensity )
            || ( m_bSimulationMode != other.m_bSimulationMode );
    }
 
private:
    tBrushMode     m_brushMode;
    GRID_COORD     m_brushSize;
    tShape         m_brushShape;
    unsigned short m_brushIntensity;
	bool           m_bSimulationMode;
};
