// NNetWinWrappers.cpp : wrapper functions for unit tests and scripting of NNet
//
// NNetWindows

#include "stdafx.h"
#include "symtab.h"
#include "Resource.h"
#include "win32_util.h"
#include "UtilityWrappers.h"
#include "NNetWrapperHelpers.h"
#include "DrawContext.h"
#include "win32_MainWindow.h"

static MainWindow * m_pMainWindow;

class WrapSetPixelOffset: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        fPixelPoint const fPixelOffset { ScrReadfPixelPoint( script ) };
        m_pMainWindow->GetDrawContext().SetPixelOffset( fPixelOffset );
    }
};

class WrapSetPixelSize: public Script_Functor
{
public:
    virtual void operator() ( Script & script ) const
    {
        MicroMeter const umPixelSize { ScrReadMicroMeter( script ) };
        m_pMainWindow->GetDrawContext().SetPixelSize( umPixelSize );
    }
};

void DefineNNetWinWrappers( MainWindow * const pMainWindow )
{
    m_pMainWindow = pMainWindow;

    DEF_FUNC( SetPixelOffset );
    DEF_FUNC( SetPixelSize );

    SymbolTable::ScrDefConst( L"ANALYZE_LOOPS",       static_cast<long>(IDM_ANALYZE_LOOPS      ) );
    SymbolTable::ScrDefConst( L"ANALYZE_ANOMALIES",   static_cast<long>(IDM_ANALYZE_ANOMALIES  ) );
    SymbolTable::ScrDefConst( L"DELETE_SELECTION",    static_cast<long>(IDM_DELETE_SELECTION   ) );
    SymbolTable::ScrDefConst( L"CLEAR_BEEPERS",       static_cast<long>(IDM_CLEAR_BEEPERS      ) );
    SymbolTable::ScrDefConst( L"SELECT_ALL_BEEPERS",  static_cast<long>(IDM_SELECT_ALL_BEEPERS ) );
    SymbolTable::ScrDefConst( L"MARK_SELECTION",      static_cast<long>(IDM_MARK_SELECTION     ) );
    SymbolTable::ScrDefConst( L"COPY_SELECTION",      static_cast<long>(IDM_COPY_SELECTION     ) );
    SymbolTable::ScrDefConst( L"INSERT_NEURON",       static_cast<long>(IDD_INSERT_NEURON      ) );
    SymbolTable::ScrDefConst( L"NEW_NEURON",          static_cast<long>(IDD_NEW_NEURON         ) );
    SymbolTable::ScrDefConst( L"NEW_INPUT_NEURON",    static_cast<long>(IDD_NEW_INPUT_NEURON   ) );
    SymbolTable::ScrDefConst( L"APPEND_NEURON",       static_cast<long>(IDD_APPEND_NEURON      ) );
    SymbolTable::ScrDefConst( L"APPEND_INPUT_NEURON", static_cast<long>(IDD_APPEND_INPUT_NEURON) );
    SymbolTable::ScrDefConst( L"ADD_OUTGOING2KNOT",   static_cast<long>(IDD_ADD_OUTGOING2KNOT  ) );
    SymbolTable::ScrDefConst( L"ADD_INCOMING2KNOT",   static_cast<long>(IDD_ADD_INCOMING2KNOT  ) );
    SymbolTable::ScrDefConst( L"ADD_OUTGOING2PIPE",   static_cast<long>(IDD_ADD_OUTGOING2PIPE  ) );
    SymbolTable::ScrDefConst( L"ADD_INCOMING2PIPE",   static_cast<long>(IDD_ADD_INCOMING2PIPE  ) );
}
