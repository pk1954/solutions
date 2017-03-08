// win32_wrappers.cpp : wrapper functions for unit tests and scripting of application
//

#include "stdafx.h"
#include "symtab.h"
#include "Resource.h"
#include "config.h"
#include "SCRIPT.H"
#include "EvoModelData.h"
#include "wrapperHelpers.h"
#include "win32_worker_thread.h"
#include "win32_winManager.h"
#include "win32_status.h"
#include "win32_appWindow.h"
#include "win32_wrappers.h"
#include "win32_util.h"

//lint -esym( 715, script )   // not referenced

static WorkThread * m_pWorkThread;
static StatusBar  * m_pStatusBar;
static BOOL         m_bMoveWindowActive;

class WrapPostDoEdit : public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        GridPoint gp = ScrReadGridPoint( script );
        m_pWorkThread->PostDoEdit( gp );
    }
};

class WrapPostSetBrushMode : public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        ULONG      const ulCmd = script.ScrReadUlong( );
        tBrushMode const mode  = static_cast<tBrushMode>( ulCmd );
        m_pWorkThread->PostSetBrushMode( mode );
    }
};

class WrapPostSetBrushShape : public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        ULONG  const ulShape = script.ScrReadUlong( );
        tShape const shape   = static_cast<tShape>( ulShape );
        m_pWorkThread->PostSetBrushShape( shape );
    }
};

class WrapPostSetBrushIntensity : public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        INT  const iValue = script.ScrReadInt( );
        m_pWorkThread->PostSetBrushIntensity( iValue );
    }
};

class WrapPostSetBrushSize : public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        INT  const iValue = script.ScrReadInt( );
        m_pWorkThread->PostSetBrushSize( iValue );
    }
};

class WrapPostNextGeneration : public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        m_pWorkThread->PostNextGeneration( );
    }
};

class WrapPostHistoryAction : public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        UINT const uiID = script.ScrReadUint( );
        m_pWorkThread->PostHistoryAction( uiID );
    }
};

class WrapPostProcessScript : public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        wstring const wstrScript = script.ScrReadString( );
        m_pWorkThread->PostProcessScript( wstrScript );
    }
};

class WrapPostRefresh : public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        m_pWorkThread->PostRefresh( );
    }
};

class WrapPostReset : public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        m_pWorkThread->PostReset();
    }
};

class WrapSetGenerationDelay : public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        DWORD const dwDelay = script.ScrReadUlong( );
        m_pWorkThread->SetGenerationDelay( dwDelay );
        m_pStatusBar->SetSpeedTrackBar( dwDelay );
    }
};

class WrapBreak : public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
    }
};

void DefineWin32WrapperFunctions
( 
    WorkThread * const pWorkThread,
    StatusBar  * const pStatusBar
)
{
    m_pWorkThread = pWorkThread;
    m_pStatusBar  = pStatusBar;

    DEF_FUNC( PostDoEdit );
    DEF_FUNC( PostSetBrushMode );
    DEF_FUNC( PostSetBrushShape );
    DEF_FUNC( PostSetBrushIntensity );
    DEF_FUNC( PostSetBrushSize );
    DEF_FUNC( PostRefresh );
    DEF_FUNC( PostReset );
    DEF_FUNC( PostNextGeneration );
    DEF_FUNC( PostHistoryAction );
    DEF_FUNC( SetGenerationDelay );
    DEF_FUNC( PostProcessScript );
    DEF_FUNC( Break );

    DEF_ULONG_CONST( IDM_GENERATION );
    DEF_ULONG_CONST( IDM_BACKWARDS );
    DEF_ULONG_CONST( IDM_RUN );
    DEF_ULONG_CONST( IDM_STOP );

    DEF_ULONG_CONST( tGenCmd::cached );
    DEF_ULONG_CONST( tGenCmd::reset );
    DEF_ULONG_CONST( tGenCmd::nextGen );
    DEF_ULONG_CONST( tGenCmd::undefined );

    DEF_ULONG_CONST( tEvoCmd::editSetBrushMode );
    DEF_ULONG_CONST( tEvoCmd::editSetBrushShape );
    DEF_ULONG_CONST( tEvoCmd::editSetBrushSize );
    DEF_ULONG_CONST( tEvoCmd::editSetBrushIntensity );
    DEF_ULONG_CONST( tEvoCmd::editSetXvalue );
    DEF_ULONG_CONST( tEvoCmd::editSetYvalue );
}
