// EditorState.h
//

#pragma once

#include "EvolutionTypes.h"
#include "gridFunctors.h"

class EditorState
{
public:

    EditorState( );
    virtual ~EditorState( ) { }

    void EditorDoEdit( Grid * const, GridPoint const );

    void SetBrushStrategy ( tBrushMode const );

    void SetBrushShape    ( tShape const shape      ) { m_shapeBrush      = shape;      }
    void SetBrushSize     ( short  const sSize      ) { m_sBrushSize      = sSize;      }
    void SetBrushIntensity( short  const sIntensity ) { m_sBrushIntensity = sIntensity; }
    
    short      GetBrushIntensity( ) const { return m_sBrushIntensity; }
    tShape     GetBrushShape( )     const { return m_shapeBrush; }
    short      GetBrushSize( )      const { return m_sBrushSize; }
    tBrushMode GetBrushMode( )      const { return m_brushMode; }

    bool operator!=( EditorState const & other ) const
    {
        return ( m_brushMode       != other.m_brushMode )
            || ( m_sBrushSize      != other.m_sBrushSize )      
            || ( m_shapeBrush      != other.m_shapeBrush )
            || ( m_sBrushIntensity != other.m_sBrushIntensity );
    }
 
private:
    tBrushMode m_brushMode;
    short      m_sBrushSize;
    tShape     m_shapeBrush;
    short      m_sBrushIntensity;
};
