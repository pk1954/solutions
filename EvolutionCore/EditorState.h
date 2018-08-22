// EditorState.h
//

#pragma once

#include "EvolutionTypes.h"
#include "BoolOp.h"
#include "Manipulator.h"

class Grid;

class EditorState
{
public:

    EditorState( );
    virtual ~EditorState( ) { }

	void Reset( );

    void EditorDoEdit( GridPoint const );

    void SetBrushShape    ( tShape     const shape ) { m_brushShape     = shape; }
    void SetBrushSize     ( GRID_COORD const size  ) { m_brushSize      = size;  }
    void SetBrushIntensity( short      const sInt  ) { m_brushIntensity = sInt;  }
    void SetSimulationMode( tBoolOp    const op    ) { ApplyOp( m_bSimulationMode, op ); }

	void SetBrushMode( Grid &, tBrushMode const );
	void SetBrushOperator ( tOperator  const ); 
  
    tOperator  GetBrushOperator ( ) const { return m_manipulator->GetOperator(); }
	short      GetBrushIntensity( ) const { return m_brushIntensity; }
    tShape     GetBrushShape( )     const { return m_brushShape; }
    GRID_COORD GetBrushSize( )      const { return m_brushSize; }
    tBrushMode GetBrushMode( )      const { return m_brushMode; }
    bool       GetSimulationMode( ) const { return m_bSimulationMode; }

    bool operator!=( EditorState const & other ) const
    {
        return ( m_manipulator     != other.m_manipulator )
            || ( m_brushMode       != other.m_brushMode )
            || ( m_brushSize       != other.m_brushSize )      
            || ( m_brushShape      != other.m_brushShape )
            || ( m_brushIntensity  != other.m_brushIntensity )
            || ( m_bSimulationMode != other.m_bSimulationMode );
    }
 
private:
	static Set_Manipulator <short> * m_setMan; 
	static Add_Manipulator <short> * m_addMan; 
	static Subt_Manipulator<short> * m_subMan;    
	static Max_Manipulator <short> * m_maxMan; 
	static Min_Manipulator <short> * m_minMan; 
	static Mean_Manipulator<short> * m_meanMan;
	
	Manipulator<short> * m_manipulator;

	GridPointFuncShort m_lambdaMode;
    tBrushMode m_brushMode;
    GRID_COORD m_brushSize;
    tShape     m_brushShape;
    short      m_brushIntensity;
	bool       m_bSimulationMode;
};
