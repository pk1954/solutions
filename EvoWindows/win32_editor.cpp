// win32_editor.cpp 
//

#include "stdafx.h"
#include <unordered_map>
#include "commctrl.h"
#include "Resource.h"
#include "config.h"
#include "EvolutionModelData.h"
#include "win32_util.h"
#include "win32_workThreadInterface.h"
#include "win32_displayOptions.h"
#include "win32_editor.h"

using namespace std;

//lint -e1924                C-style cast

EditorWindow::EditorWindow( )
  : BaseDialog( ),
    m_pModelWork( nullptr ),
    m_pWorkThreadInterface( nullptr ),
    m_pDspOptWindow( nullptr )
{ }

void EditorWindow::Start
(  
    HWND                  const hWndParent,
    WorkThreadInterface * const pWorkThreadInterface,
    EvolutionModelData  * const pModel,
    DspOptWindow        * const pDspOptWindow
    )
{
    StartBaseDialog( hWndParent, MAKEINTRESOURCE( IDD_EDITOR ) );
    m_pWorkThreadInterface = pWorkThreadInterface;
    m_pModelWork    = pModel;
    m_pDspOptWindow = pDspOptWindow;

    SetTrackBarRange( IDM_EDIT_SIZE,      1,  50 );
    SetTrackBarRange( IDM_EDIT_INTENSITY, 1, 100 );
    UpdateEditControls( );
}

EditorWindow::~EditorWindow( )
{
    m_pDspOptWindow        = nullptr;
    m_pWorkThreadInterface = nullptr;
    m_pModelWork           = nullptr;
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

void EditorWindow::UpdateEditControls( )
{
	// Set state of all editor window widgets according to model

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
    
		CheckRadioButton( IDM_MOVE, IDM_FOOD_STOCK, mapModeTable.at( m_pModelWork->GetBrushMode () ) );
	}
	{
		static unordered_map < tShape, WORD > mapShapeTable =
		{
			{ tShape::Circle, IDM_EDIT_CIRCLE    },    
			{ tShape::Rect,   IDM_EDIT_RECTANGLE }
		};

		CheckRadioButton( IDM_EDIT_CIRCLE, IDM_EDIT_RECTANGLE, mapShapeTable.at( m_pModelWork->GetBrushShape() ) );
	}

	SetTrackBarPos( IDM_EDIT_SIZE,      static_cast<long>( m_pModelWork->GetBrushSize( )) );
    SetTrackBarPos( IDM_EDIT_INTENSITY, static_cast<long>( m_pModelWork->GetBrushIntensity( ) + 10) );

	// adjust display options window according to state of editor

	m_pDspOptWindow->UpdateDspOptionsControls( m_pModelWork->GetBrushMode () );
}

INT_PTR EditorWindow::UserProc( UINT const message, WPARAM const wParam, LPARAM const lParam )
{
    switch (message)
    {
    case WM_HSCROLL:
		{
			HWND   hwndTrackBar = (HWND)lParam;
			USHORT usLogicalPos = static_cast<USHORT>(::SendMessage( hwndTrackBar, TBM_GETPOS, 0, 0 ));
			switch ( GetDlgCtrlID( hwndTrackBar ) )
			{
			case IDM_EDIT_INTENSITY:
				m_pWorkThreadInterface->PostSetBrushIntensity( usLogicalPos );
				break;
			case IDM_EDIT_SIZE:
				m_pWorkThreadInterface->PostSetBrushSize( usLogicalPos );
				break;
			default:
				assert( false );
			}
		}
        return TRUE;

    case WM_COMMAND:
        {
            WORD const wId = LOWORD( wParam );

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
				{
					static unordered_map < WORD, tBrushMode > mapModeTable =
					{
						{ IDM_MOVE,            tBrushMode::move            },
						{ IDM_RANDOM_STRATEGY, tBrushMode::randomStrategy  },
						{ IDM_COOPERATE,       tBrushMode::cooperate       },
						{ IDM_DEFECT,          tBrushMode::defect          },
						{ IDM_TIT4TAT,         tBrushMode::tit4tat         },
						{ IDM_KILL_ANIMALS,    tBrushMode::noAnimals       },
						{ IDM_MUT_RATE,        tBrushMode::mutRate         },
						{ IDM_FERTILITY,       tBrushMode::fertility       },
						{ IDM_FOOD_STOCK,      tBrushMode::food            },
						{ IDM_FERTILIZER,      tBrushMode::fertilizer      }
					};

					tBrushMode const brushMode = mapModeTable.at( wId ) ;
					m_pWorkThreadInterface->PostSetBrushMode( brushMode );
					m_pDspOptWindow->UpdateDspOptionsControls( brushMode );
				}
                break;

            case IDM_EDIT_CIRCLE:
            case IDM_EDIT_RECTANGLE:
				{
					static unordered_map < WORD, tShape > mapShapeTable =
					{
						{ IDM_EDIT_CIRCLE,    tShape::Circle },    
						{ IDM_EDIT_RECTANGLE, tShape::Rect   }
					};

					m_pWorkThreadInterface->PostSetBrushShape( mapShapeTable.at( wId ) );
				}
                break;

            default:
                break;
            }
        }
        break;

    case WM_CLOSE:
        Show( FALSE );
        return TRUE;

    case WM_DESTROY:
        DestroyWindow( GetWindowHandle( ) );
        break;

    default:
        break;
    }

    return FALSE;
}
