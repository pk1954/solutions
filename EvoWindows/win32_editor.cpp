// win32_editor.cpp 
//

#include "stdafx.h"
#include <unordered_map>
#include "commctrl.h"
#include "Resource.h"
#include "config.h"
#include "EvolutionCore.h"
#include "win32_util.h"
#include "win32_status.h"
#include "win32_workThreadInterface.h"
#include "win32_displayOptions.h"
#include "win32_editor.h"

EditorWindow::EditorWindow( )
  : BaseDialog( ),
    m_pCore               ( nullptr ),
    m_pWorkThreadInterface( nullptr ),
    m_pDspOptWindow       ( nullptr )
{ }

void EditorWindow::Start
(  
    HWND                  const hwndParent,
    WorkThreadInterface * const pWorkThreadInterface,
    EvolutionCore       * const pCore,
    DspOptWindow        * const pDspOptWindow
)
{
    m_pWorkThreadInterface = pWorkThreadInterface;
    m_pCore                = pCore;
    m_pDspOptWindow        = pDspOptWindow;

    StartBaseDialog( hwndParent, MAKEINTRESOURCE( IDD_EDITOR ) );

    SetTrackBarRange( IDM_EDIT_SIZE,      1L,  50L );
    SetTrackBarRange( IDM_EDIT_INTENSITY, 0L, 100L );
    UpdateEditControls( );
}

void EditorWindow::Stop( )
{
	DestroyWindow( GetWindowHandle( ) );
	m_pWorkThreadInterface = nullptr;
	m_pCore                = nullptr;
	m_pDspOptWindow        = nullptr;
}

EditorWindow::~EditorWindow( )
{
    m_pWorkThreadInterface = nullptr;
    m_pCore                = nullptr;
    m_pDspOptWindow        = nullptr;
}

void EditorWindow::UpdateEditControls( )
{
	updateEditControls( m_pCore );
}

LRESULT EditorWindow::sendClick( int const item ) const
{
	HWND    const hwndOld { SetActiveWindow( GetWindowHandle( ) ) };
	LRESULT const res     { SendDlgItemMessage( item, BM_CLICK, 0, 0 ) };
    (void)SetActiveWindow( hwndOld );
    return res;
}

void EditorWindow::updateOperationButtons( tBrushMode const mode ) const
{
	bool bEnableOperationButtons = ! IsStrategyBrushMode( mode );

	EnableWindow( GetDlgItem( IDM_EDIT_OPERATION_SET ),      bEnableOperationButtons );
	EnableWindow( GetDlgItem( IDM_EDIT_OPERATION_MIN ),      bEnableOperationButtons );
	EnableWindow( GetDlgItem( IDM_EDIT_OPERATION_MAX ),      bEnableOperationButtons );
	EnableWindow( GetDlgItem( IDM_EDIT_OPERATION_ADD ),      bEnableOperationButtons );
	EnableWindow( GetDlgItem( IDM_EDIT_OPERATION_SUBTRACT ), bEnableOperationButtons );
}

void EditorWindow::updateEditControls( EvolutionCore const * const pCore ) // Set state of all window widgets according to mode (edit/simu)
{
	static std::unordered_map < tBrushMode, WORD > mapModeTable
	{
		{ tBrushMode::move,        IDM_MOVE            },
		{ tBrushMode::randomStrat, IDM_RANDOM_STRATEGY },
		{ tBrushMode::cooperate,   IDM_COOPERATE       },
		{ tBrushMode::defect,      IDM_DEFECT          },
		{ tBrushMode::tit4tat,     IDM_TIT4TAT         },
		{ tBrushMode::noAnimals,   IDM_KILL_ANIMALS    },
		{ tBrushMode::mutRate,     IDM_MUT_RATE        },
		{ tBrushMode::fertility,   IDM_FERTILITY       },
		{ tBrushMode::food,        IDM_FOOD_STOCK      },
		{ tBrushMode::fertilizer,  IDM_FERTILIZER      }
	};
    
	CheckRadioButton( IDM_MOVE, IDM_FOOD_STOCK, mapModeTable.at( pCore->GetBrushMode() ) );

	static std::unordered_map < tShape, WORD > mapShapeTable
	{
		{ tShape::Circle, IDM_EDIT_CIRCLE    },    
		{ tShape::Rect,   IDM_EDIT_RECTANGLE },
		{ tShape::Grid,   IDM_EDIT_GRID_AREA }
	};

	CheckRadioButton( IDM_EDIT_CIRCLE, IDM_EDIT_GRID_AREA, mapShapeTable.at( pCore->GetBrushShape() ) );

	// ShowWindow  ( GetDlgItem( IDM_EDIT_SIZE ), false );

	updateOperationButtons( pCore->GetBrushMode() );

	static std::unordered_map < tManipulator, WORD > mapOperatorTable
	{
		{ tManipulator::set,      IDM_EDIT_OPERATION_SET      },    
		{ tManipulator::min,      IDM_EDIT_OPERATION_MIN      },
		{ tManipulator::max,      IDM_EDIT_OPERATION_MAX      },
		{ tManipulator::add,      IDM_EDIT_OPERATION_ADD      },
		{ tManipulator::subtract, IDM_EDIT_OPERATION_SUBTRACT },
	};

	CheckRadioButton( IDM_EDIT_OPERATION_SET, IDM_EDIT_OPERATION_SUBTRACT, mapOperatorTable.at( pCore->GetBrushManipulator() ) );
	
	SetTrackBarPos( IDM_EDIT_SIZE,      pCore->GetBrushSize().GetValue() );
    SetTrackBarPos( IDM_EDIT_INTENSITY, pCore->GetBrushIntensity().GetValue() );

	// adjust display options window

	m_pDspOptWindow->UpdateDspOptionsControls( pCore->GetBrushMode() );
}

void EditorWindow::SetSimuMode( bool const bSimulationMode )
{
	if ( bSimulationMode )
		sendClick( IDM_MOVE );
}

void EditorWindow::setBrushMode( WORD const wId ) const
{
	static std::unordered_map < WORD, tBrushMode > mapModeTable
	{
		{ IDM_MOVE,            tBrushMode::move        },
		{ IDM_RANDOM_STRATEGY, tBrushMode::randomStrat },
		{ IDM_COOPERATE,       tBrushMode::cooperate   },
		{ IDM_DEFECT,          tBrushMode::defect      },
		{ IDM_TIT4TAT,         tBrushMode::tit4tat     },
		{ IDM_KILL_ANIMALS,    tBrushMode::noAnimals   },
		{ IDM_MUT_RATE,        tBrushMode::mutRate     },
		{ IDM_FERTILITY,       tBrushMode::fertility   },
		{ IDM_FOOD_STOCK,      tBrushMode::food        },
		{ IDM_FERTILIZER,      tBrushMode::fertilizer  }
	};

	tBrushMode const brushMode { mapModeTable.at( wId ) };
	m_pWorkThreadInterface->PostSetBrushMode( brushMode );
}

void EditorWindow::setBrushShape( WORD const wId ) const
{
	static std::unordered_map < WORD, tShape > mapShapeTable
	{
		{ IDM_EDIT_CIRCLE,    tShape::Circle },    
		{ IDM_EDIT_RECTANGLE, tShape::Rect   },
		{ IDM_EDIT_GRID_AREA, tShape::Grid   }
	};

	tShape const brushShape { mapShapeTable.at( wId ) };
	m_pWorkThreadInterface->PostSetBrushShape( brushShape );
}

void EditorWindow::setBrushManipulator( WORD const wId ) const
{
	static std::unordered_map < WORD, tManipulator > mapOperationTable
	{
		{ IDM_EDIT_OPERATION_SET,      tManipulator::set      },    
		{ IDM_EDIT_OPERATION_MIN,      tManipulator::min      },    
		{ IDM_EDIT_OPERATION_MAX,      tManipulator::max      },    
		{ IDM_EDIT_OPERATION_ADD,      tManipulator::add      },    
		{ IDM_EDIT_OPERATION_SUBTRACT, tManipulator::subtract },    
	};

	tManipulator const brushOperator { mapOperationTable.at( wId ) };
	m_pWorkThreadInterface->PostSetBrushManipulator( brushOperator );
}

INT_PTR EditorWindow::UserProc( UINT const message, WPARAM const wParam, LPARAM const lParam )
{
    switch (message)
    {
    case WM_HSCROLL:
		{
			HWND  const hwndTrackBar { (HWND)lParam };
			int   const iCtrlId      { GetDlgCtrlID( hwndTrackBar ) };
			short const sLogicalPos  { GetTrackBarPos( iCtrlId ) };
			switch ( iCtrlId )
			{
			case IDM_EDIT_INTENSITY:
				m_pWorkThreadInterface->PostSetBrushIntensity( PERCENT(sLogicalPos) );
				break;
			case IDM_EDIT_SIZE:
				m_pWorkThreadInterface->PostSetBrushRadius( GRID_COORD(sLogicalPos) );
				break;
			default:
				assert( false );
			}
		}
        return TRUE;

	case WM_ACTIVATE:
		if ( LOWORD( wParam ) == WA_CLICKACTIVE )  
			PostCommand2Application( IDM_EDIT_MODE, 0 );
		return FALSE;

    case WM_COMMAND:
        {
			WORD const wId { LOWORD( wParam ) };

            switch ( wId )
            {
            case IDM_MOVE:
            case IDM_RANDOM_STRATEGY:
            case IDM_COOPERATE:
            case IDM_DEFECT:
            case IDM_TIT4TAT:
            case IDM_KILL_ANIMALS:
            case IDM_MUT_RATE:
            case IDM_FERTILITY:
            case IDM_FOOD_STOCK:
				setBrushMode( wId );
                break;

            case IDM_EDIT_CIRCLE:
            case IDM_EDIT_RECTANGLE:
            case IDM_EDIT_GRID_AREA:
				setBrushShape( wId );
                break;

			case IDM_EDIT_OPERATION_SET:     
			case IDM_EDIT_OPERATION_MIN:     
			case IDM_EDIT_OPERATION_MAX:     
			case IDM_EDIT_OPERATION_ADD:     
			case IDM_EDIT_OPERATION_SUBTRACT:
				setBrushManipulator( wId );
                break;

            default:
				assert( false );
                break;
            }
        }
        break;

    case WM_CLOSE:
        Show( FALSE );
        return TRUE;

    default:
        break;
    }

    return FALSE;
}
