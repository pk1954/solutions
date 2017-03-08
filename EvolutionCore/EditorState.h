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

    void InitEditorState( Grid * const );
    void DoEdit( GridPoint const );

    void SetBrushStrategy ( tBrushMode const );

    void EditorState::SetBrushShape    ( tShape const shape      ) { m_shapeBrush      = shape;      }
    void EditorState::SetBrushSize     ( short  const sSize      ) { m_sBrushSize      = sSize;      }
    void EditorState::SetBrushIntensity( short  const sIntensity ) { m_sBrushIntensity = sIntensity; }
    
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
    static FoodStock_Functor  m_FoodStock_Functor;
    static Fertilizer_Functor m_Fertilizer_Functor;
    static Fertility_Functor  m_Fertility_Functor;
    static MutRate_Functor    m_MutRate_Functor;
    static Strategy_Functor   m_Strategy_Functor;

    tBrushMode m_brushMode;
    short      m_sBrushSize;
    tShape     m_shapeBrush;
    short      m_sBrushIntensity;
};
