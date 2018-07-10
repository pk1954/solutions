// win32_editor.cpp 
//

#include "stdafx.h"
#include <unordered_map>
#include "commctrl.h"
#include "Resource.h"
#include "config.h"
#include "EvolutionModelData.h"
#include "win32_util.h"
#include "win32_worker_thread.h"
#include "win32_displayOptions.h"
#include "win32_editor.h"

using namespace std;

//lint -e1924                C-style cast

EditorWindow::EditorWindow( )
  : BaseDialog( ),
    m_pModelWork( nullptr ),
    m_pWorkThread( nullptr ),
    m_pDspOptWindow( nullptr )
{ }

void EditorWindow::Start
(  
    HWND                 const hWndParent,
    WorkThread         * const pWorkThread,
    EvolutionModelData * const pModel,
    DspOptWindow       * const pDspOptWindow
    )
{
    StartBaseDialog( hWndParent, MAKEINTRESOURCE( IDD_EDITOR ) );
    m_pWorkThread   = pWorkThread;
    m_pModelWork    = pModel;
    m_pDspOptWindow = pDspOptWindow;

    (void)SendDlgItemMessage( IDM_EDIT_SIZE,      TBM_SETRANGE, TRUE, MAKELONG( 1,  50 ) );
    (void)SendDlgItemMessage( IDM_EDIT_INTENSITY, TBM_SETRANGE, TRUE, MAKELONG( 1, 100 ) );
    UpdateControls( );
}

EditorWindow::~EditorWindow( )
{
    m_pDspOptWindow = nullptr;
    m_pWorkThread   = nullptr;
    m_pModelWork    = nullptr;
}

BOOL EditorWindow::IsInEditMode( ) const 
{ 
	return m_pModelWork->GetBrushMode( ) != tBrushMode::move; 
};

LRESULT EditorWindow::SendClick( int const item ) const
{
    HWND    const hwndOld = SetActiveWindow( GetWindowHandle( ) );
    LRESULT const res = SendDlgItemMessage( item, BM_CLICK, 0, 0 );
    (void)SetActiveWindow( hwndOld );
    return res;
}
    
void EditorWindow::UpdateControls( )
{
    static unordered_map < tBrushMode, WORD > mapModeTable =
    {
        { tBrushMode::move,           IDM_MOVE            },
        { tBrushMode::randomStrategy, IDM_RANDOM_STRATEGY },
        { tBrushMode::cooperate,      IDM_COOPERATE       },
        { tBrushMode::defect,         IDM_DEFECT          },
        { tBrushMode::tit4tat,        IDM_TIT4TAT         },
        { tBrushMode::noAnimals,      IDM_KILL_ANIMALS    },
        { tBrushMode::mutRate,        IDM_MUT_RATE        },
        { tBrushMode::fertility,      IDM_FERTILITY       },
        { tBrushMode::food,           IDM_FOOD_STOCK      },
        { tBrushMode::fertilizer,     IDM_FERTILIZER      }
    };

    static unordered_map < tBrushMode, WORD > mapDspOptTable =
    {
        { tBrushMode::move,           IDM_MOVE       },
        { tBrushMode::randomStrategy, IDM_ANIMALS    },
        { tBrushMode::cooperate,      IDM_ANIMALS    },
        { tBrushMode::defect,         IDM_ANIMALS    },
        { tBrushMode::tit4tat,        IDM_ANIMALS    },
        { tBrushMode::noAnimals,      IDM_ANIMALS    },
        { tBrushMode::mutRate,        IDM_MUT_RATE   },
        { tBrushMode::fertility,      IDM_FERTILITY  },
        { tBrushMode::food,           IDM_FOOD_STOCK },
        { tBrushMode::fertilizer,     IDM_FERTILIZER }
    };

    static unordered_map < tShape, WORD > mapShapeTable =
    {
        { tShape::Circle, IDM_EDIT_CIRCLE    },    
        { tShape::Rect,   IDM_EDIT_RECTANGLE }
    };

    int const iShapeButtonId = static_cast<int>( mapShapeTable.at( m_pModelWork->GetBrushShape( ) ) );
    int const iModeButtonId  = static_cast<int>( mapModeTable.at ( m_pModelWork->GetBrushMode ( ) ) );

    CheckRadioButton( IDM_MOVE,        IDM_FOOD_STOCK,     iModeButtonId  );
    CheckRadioButton( IDM_EDIT_CIRCLE, IDM_EDIT_RECTANGLE, iShapeButtonId );
    
    if ( IsInEditMode( ) )
    {
        WORD const wDspOptId = mapDspOptTable.at( m_pModelWork->GetBrushMode( ) );
        if ( wDspOptId == IDM_ANIMALS )
            m_pDspOptWindow->SetIndividualsVisible( );
        else 
            m_pDspOptWindow->SetDisplayMode( wDspOptId );
    }

    (void)SendDlgItemMessage( IDM_EDIT_SIZE,      TBM_SETPOS, TRUE, static_cast<long>( m_pModelWork->GetBrushSize( )) );
    (void)SendDlgItemMessage( IDM_EDIT_INTENSITY, TBM_SETPOS, TRUE, static_cast<long>( m_pModelWork->GetBrushIntensity( ) + 10) );
}

INT_PTR EditorWindow::UserProc( UINT const message, WPARAM const wParam, LPARAM const lParam )
{
    switch (message)
    {
    case WM_HSCROLL:
        switch( LOWORD( wParam ) )
        {
            case TB_THUMBTRACK:
            case TB_THUMBPOSITION:
            {
                int const iLogicalPos = HIWORD( wParam );
                int const iCtrlId     = GetDlgCtrlID( (HWND)lParam );
                if ( iCtrlId == IDM_EDIT_INTENSITY )
                    m_pWorkThread->PostSetBrushIntensity( iLogicalPos );
                else if ( iCtrlId == IDM_EDIT_SIZE )
                    m_pWorkThread->PostSetBrushSize( iLogicalPos );
                else
                    assert( false );
                break;
            }
            default:
                break;
        }
        return TRUE;

    case WM_COMMAND:
        {
            WORD const wId = LOWORD( wParam );

            switch ( wId )
            {
            case IDM_MOVE:
                m_pWorkThread->PostSetBrushMode( tBrushMode::move );
                break;

            case IDM_RANDOM_STRATEGY:
                m_pWorkThread->PostSetBrushMode( tBrushMode::randomStrategy );
                break;

            case IDM_COOPERATE:
                m_pWorkThread->PostSetBrushMode( tBrushMode::cooperate );
                break;

            case IDM_DEFECT:
                m_pWorkThread->PostSetBrushMode( tBrushMode::defect );
                break;

            case IDM_TIT4TAT:
                m_pWorkThread->PostSetBrushMode( tBrushMode::tit4tat );
                break;

            case IDM_KILL_ANIMALS:
                m_pWorkThread->PostSetBrushMode( tBrushMode::noAnimals );
                break;

            case IDM_MUT_RATE:
                m_pWorkThread->PostSetBrushMode( tBrushMode::mutRate  );
                break;

            case IDM_FERTILITY:
                m_pWorkThread->PostSetBrushMode( tBrushMode::fertility  );
                break;

            case IDM_FOOD_STOCK:
                m_pWorkThread->PostSetBrushMode( tBrushMode::food );
                break;

            case IDM_EDIT_CIRCLE:
                m_pWorkThread->PostSetBrushShape( tShape::Circle );
                break;

            case IDM_EDIT_RECTANGLE:
                m_pWorkThread->PostSetBrushShape( tShape::Rect );
                break;

            default:
                break;
            }
        }
        break;

    case WM_CLOSE:
        Show( FALSE );
        return (INT_PTR)TRUE;

    case WM_DESTROY:
        DestroyWindow( GetWindowHandle( ) );
        break;

    default:
        break;
    }

    return static_cast<INT_PTR>(FALSE);
}
