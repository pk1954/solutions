// win32_displayOptions.cpp 
//

#include "stdafx.h"
#include <unordered_map>
#include <functional>
#include "Resource.h"
#include "windowsx.h"
#include "gridNeighbor.h"
#include "EvolutionCore.h"
#include "win32_util.h"
#include "win32_baseDialog.h"
#include "win32_displayOptions.h"

DspOptWindow::DspOptWindow(  ) : 
	BaseDialog( )
{ }

DspOptWindow::~DspOptWindow( )
{
	m_IntValueLambda = nullptr;
}

void DspOptWindow::Start( HWND const hwndParent )
{
    StartBaseDialog( hwndParent, MAKEINTRESOURCE( IDD_DISPLAY_OPTIONS ) );
	m_IntValueLambda = nullptr;

	//{
	//	HWND hwndDlg = GetWindowHandle( );
	//	HWND hwndTip = CreateWindowEx
	//				  (
	//					  WS_EX_TOPMOST, TOOLTIPS_CLASS, nullptr,
	//			          WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP,
	//					  CW_USEDEFAULT, CW_USEDEFAULT,
	//					  CW_USEDEFAULT, CW_USEDEFAULT,
	//					  hwndDlg, nullptr, GetModuleHandle( nullptr ),
	//					  nullptr
	//				  );
	//	assert( hwndTip != nullptr );

	//	SetWindowPos
	//	(
	//		hwndTip, HWND_TOPMOST, 0, 0, 0, 0,
	//		SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE
	//	);

	//	PTSTR pszText = L"Fyxyxyxyxyx";

	//	// Associate the tooltip with the tool.
	//	TOOLINFO toolInfo = { 0 };
	//	toolInfo.cbSize = sizeof(toolInfo);
	//	toolInfo.hwnd = hwndTip;
	//	toolInfo.uFlags = TTF_ABSOLUTE | TTF_IDISHWND;
	//	toolInfo.uId = (UINT_PTR)hwndTip;
	//	toolInfo.lpszText = pszText;
	//	::GetClientRect (hwndDlg, &toolInfo.rect);

	//	LRESULT lres = ::SendMessage( hwndTip, TTM_ADDTOOL, 0, (LPARAM)&toolInfo );
	//}
}

void DspOptWindow::Stop( )
{
	DestroyWindow( GetWindowHandle( ) );
}

void DspOptWindow::UpdateDspOptionsControls( tBrushMode const brushMode )
{
	if ( brushMode != tBrushMode::move )
	{
		static std::unordered_map < tBrushMode, WORD > mapDspOptTable =
		{
			{ tBrushMode::randomStrat, IDM_ANIMALS    },
			{ tBrushMode::cooperate,   IDM_ANIMALS    },
			{ tBrushMode::defect,      IDM_ANIMALS    },
			{ tBrushMode::tit4tat,     IDM_ANIMALS    },
			{ tBrushMode::noAnimals,   IDM_ANIMALS    },
			{ tBrushMode::mutRate,     IDM_MUT_RATE   },
			{ tBrushMode::fertility,   IDM_FERTILITY  },
			{ tBrushMode::food,        IDM_FOOD_STOCK },
			{ tBrushMode::fertilizer,  IDM_FERTILIZER }
		};
		WORD const wDspOptId = mapDspOptTable.at( brushMode );
		if ( wDspOptId == IDM_ANIMALS )
			SetIndividualsVisible( );
		else 
			SetDisplayMode( wDspOptId );
	}
}

void DspOptWindow::SetDisplayMode( WORD const wMode )
{ 
    HWND const hDlgItem = GetDlgItem( wMode );
    assert( hDlgItem != nullptr );

    (void)::SendMessage( hDlgItem, BM_CLICK, 0 , 0);
};

void DspOptWindow::SetIndividualsVisible( )
{
	Button_SetCheck( GetDlgItem( IDM_ANIMALS), BST_CHECKED );
}

BOOL DspOptWindow::AreIndividualsVisible( ) const
{
    return ( BST_CHECKED == Button_GetCheck( GetDlgItem( IDM_ANIMALS ) ) );
}

INT_PTR DspOptWindow::UserProc( UINT const message, WPARAM const wParam, LPARAM const lParam )
{
    switch (message)
    {
    case WM_INITDIALOG:
		Button_SetCheck( GetDlgItem( IDM_ANIMALS   ), BST_CHECKED );
		Button_SetCheck( GetDlgItem( IDM_FOOD_STOCK), BST_CHECKED );
        return TRUE;

    case WM_COMMAND:
        {
            WORD const wId = LOWORD(wParam);
            switch ( wId )
            {
            case IDM_MUT_RATE:
				m_IntValueLambda = [&](EvolutionCore const * const pCore, GridPoint const gp){ return pCore->GetMutRate( gp ).GetValue(); };
                break;

            case IDM_FERTILITY:
				m_IntValueLambda = [&](EvolutionCore const * const pCore, GridPoint const gp){ return pCore->GetFertility( gp ).GetValue(); };
                break;

            case IDM_FOOD_STOCK:
				m_IntValueLambda = [&](EvolutionCore const * const pCore, GridPoint const gp){ return pCore->GetFoodStock( gp ).GetValue(); };
                break;

            case IDM_FERTILIZER:
				m_IntValueLambda = [&](EvolutionCore const * const pCore, GridPoint const gp){ return pCore->GetFertilizer( gp ).GetValue(); };
                break;

            case IDM_DSP_ENV_NOTHING:
				m_IntValueLambda = [&](EvolutionCore const * const pCore, GridPoint const gp){ return 0; };
                break;

            case IDCANCEL:
                Show( FALSE );
                return TRUE;

            default:
                break;
            }
			PostCommand2Application( IDM_REFRESH, 0 );
        }
        break;

    default:
        break;
    }

    return FALSE;
}
