// EditorState.h
//

#pragma once

#include "EvolutionTypes.h"

class Grid;

class EditorState
{
public:

    EditorState( );
    virtual ~EditorState( ) { }

	void Reset( );

    void EditorDoEdit( Grid * const, GridPoint const );

    void SetBrushStrategy ( tBrushMode const );

    void SetBrushShape    ( tShape         const shape       ) { m_shapeBrush       = shape;       }
    void SetBrushSize     ( GRID_COORD     const size        ) { m_brushSize        = size;        }
    void SetBrushIntensity( unsigned short const usIntensity ) { m_usBrushIntensity = usIntensity; }
    
    unsigned short GetBrushIntensity( ) const { return m_usBrushIntensity; }
    tShape         GetBrushShape( )     const { return m_shapeBrush; }
    GRID_COORD     GetBrushSize( )      const { return m_brushSize; }
    tBrushMode     GetBrushMode( )      const { return m_brushMode; }

    bool operator!=( EditorState const & other ) const
    {
        return ( m_brushMode        != other.m_brushMode )
            || ( m_brushSize        != other.m_brushSize )      
            || ( m_shapeBrush       != other.m_shapeBrush )
            || ( m_usBrushIntensity != other.m_usBrushIntensity );
    }
 
private:
    tBrushMode     m_brushMode;
    GRID_COORD     m_brushSize;
    tShape         m_shapeBrush;
    unsigned short m_usBrushIntensity;
};
