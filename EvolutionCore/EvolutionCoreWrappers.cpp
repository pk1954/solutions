// EvolutionCoreWrappers.cpp : wrapper functions for unit tests and scripting of application
//

#include "stdafx.h"
#include "assert.h"
#include "SCRIPT.H"
#include "symtab.h"
#include "EvolutionModelData.h"
#include "EvolutionCore.h"
#include "EvolutionTypes.h"
#include "EditorState.h"
#include "gridCircle.h"
#include "pixelCoordinates.h"
#include "EvolutionCoreWrapperHelpers.h"
#include "EvolutionCoreWrappers.h"

//lint -esym( 715, script )  // not referenced

static EvolutionCore      * m_pCore;
static EvolutionModelData * m_pModelWork;
static PixelCoordinates   * m_pPixCoords;

class WrapSetPoi : public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        GridPoint gp = ScrReadGridPoint( script );
        m_pModelWork->SetPoi( gp );
    }
};

class WrapClearPoi : public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        m_pModelWork->ClearPoi( );
    }
};

class WrapDumpGridPointList : public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        m_pCore->DumpGridPointList( m_pModelWork );
    }
};

class WrapCompute : public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        m_pCore->Compute( m_pModelWork );
    }
};

class WrapResetModel : public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        m_pModelWork->ResetAll( );
    }
};

class WrapPause : public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    { }
};

class WrapModelDoEdit : public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        GridPoint gp = ScrReadGridPoint( script );
        m_pModelWork->ModelDoEdit( gp );
    }
};

class WrapSetBrushShape : public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        int    const iShape = script.ScrReadInt( );
        tShape const shape  = static_cast<tShape>( iShape );
        m_pModelWork->SetBrushShape( shape );
    }
};

class WrapSetBrushSize : public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        unsigned char size = script.ScrReadUchar( );
        m_pModelWork->SetBrushSize( size );
    }
};

class WrapSetBrushIntensity : public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        short sIntensity = script.ScrReadShort( );
        m_pModelWork->SetBrushIntensity( sIntensity );
    }
};

class WrapSetBrushStrategy : public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        int        const iMode = script.ScrReadInt( );
        tBrushMode const mode  = static_cast<tBrushMode>( iMode );
        m_pModelWork->SetBrushStrategy( mode );
    }
};

class WrapPixel2GridPos : public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        PixelPoint const pixPos = ScrReadPixelPoint( script );
		GridPoint  const gp     = m_pPixCoords->Pixel2GridPos( pixPos );
		wcout << gp << endl;
    }
};

class WrapSetFieldSize : public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
		short      const sFieldSize = script.ScrReadShort();
        PixelPoint const pixPos     = ScrReadPixelPoint( script );
		m_pPixCoords->SetGridFieldSize( sFieldSize, pixPos );
    }
};

class WrapMoveGrid : public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        PixelPoint const pixPosDelta = ScrReadPixelPoint( script );
		m_pPixCoords->MoveGrid( pixPosDelta );
    }
};

void DefinePixelCoordinatesWrapperFunctions( PixelCoordinates * pCoords )
{
    m_pPixCoords = pCoords;

    DEF_FUNC( Pixel2GridPos );
    DEF_FUNC( SetFieldSize );
    DEF_FUNC( MoveGrid );
}

void DefineModelWrapperFunctions( EvolutionModelData * pModel )
{
    m_pModelWork = pModel;

    DEF_FUNC( ModelDoEdit );
    DEF_FUNC( SetBrushShape );
    DEF_FUNC( SetBrushSize );
    DEF_FUNC( SetBrushIntensity );
    DEF_FUNC( SetBrushStrategy );
}

void DefineCoreWrapperFunctions( EvolutionCore * pCore )
{
    m_pCore = pCore;

	DEF_FUNC( Compute );
	DEF_FUNC( DumpGridPointList );
    DEF_FUNC( ResetModel );
    DEF_FUNC( SetPoi );
    DEF_FUNC( ClearPoi );
    DEF_FUNC( Include );
    DEF_FUNC( Pause );

    DEF_ULONG_CONST( tStrategyId::defectAlways );
    DEF_ULONG_CONST( tStrategyId::cooperateAlways );
    DEF_ULONG_CONST( tStrategyId::tit4tat );

    DEF_ULONG_CONST( tShape::Rect );
    DEF_ULONG_CONST( tShape::Circle );

    DEF_ULONG_CONST( tBrushMode::move );
    DEF_ULONG_CONST( tBrushMode::randomStrategy );
    DEF_ULONG_CONST( tBrushMode::cooperate );
    DEF_ULONG_CONST( tBrushMode::defect );
    DEF_ULONG_CONST( tBrushMode::tit4tat );
    DEF_ULONG_CONST( tBrushMode::noAnimals );
    DEF_ULONG_CONST( tBrushMode::mutRate );
    DEF_ULONG_CONST( tBrushMode::fertility );
    DEF_ULONG_CONST( tBrushMode::food );
    DEF_ULONG_CONST( tBrushMode::fertilizer );
}
