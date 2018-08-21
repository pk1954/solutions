// win32_editorWrappers.cpp : wrapper functions for unit tests and scripting of application
//

#include "stdafx.h"
#include "symtab.h"
#include "SCRIPT.H"
#include "Resource.h"
#include "EvolutionCoreWrapperHelpers.h"
#include "EvolutionTypes.h"
#include "win32_editor.h"
#include "win32_editorWrappers.h"

static EditorWindow const * m_pEditorWindow;
 
void DefineWin32EditorWrapperFunctions( EditorWindow const * const pEditorWindow )
{
    m_pEditorWindow = pEditorWindow;

    DEF_ULONG_CONST( IDCANCEL );

    DEF_ULONG_CONST( IDM_EDIT_SIZE );
    DEF_ULONG_CONST( IDM_EDIT_INTENSITY );

    DEF_ULONG_CONST( IDM_EDIT_CIRCLE );
    DEF_ULONG_CONST( IDM_EDIT_RECTANGLE );

    DEF_ULONG_CONST( IDM_EDIT_OPERATION_SET );
    DEF_ULONG_CONST( IDM_EDIT_OPERATION_MIN );
    DEF_ULONG_CONST( IDM_EDIT_OPERATION_MAX );
    DEF_ULONG_CONST( IDM_EDIT_OPERATION_ADD );
    DEF_ULONG_CONST( IDM_EDIT_OPERATION_SUBTRACT );
    DEF_ULONG_CONST( IDM_EDIT_OPERATION_MEAN );

    DEF_ULONG_CONST( IDM_MOVE );
    DEF_ULONG_CONST( IDM_RANDOM_STRATEGY );
    DEF_ULONG_CONST( IDM_COOPERATE );
    DEF_ULONG_CONST( IDM_DEFECT );
    DEF_ULONG_CONST( IDM_TIT4TAT );
    DEF_ULONG_CONST( IDM_KILL_ANIMALS );
    DEF_ULONG_CONST( IDM_MUT_RATE );
    DEF_ULONG_CONST( IDM_FERTILITY );
    DEF_ULONG_CONST( IDM_FOOD_STOCK );
    DEF_ULONG_CONST( IDM_FERTILIZER );
}
