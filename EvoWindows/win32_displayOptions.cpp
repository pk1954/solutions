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

DspOptWindow::DspOptWindow(  )
    : BaseDialog( )
{ }

DspOptWindow::~DspOptWindow( )
{
	m_IntValueLambda = nullptr;
}

void DspOptWindow::Start
( 
    HWND          const         hWndParent, 
    EvolutionCore const * const pCore 
)
{
    StartBaseDialog( hWndParent, MAKEINTRESOURCE( IDD_DISPLAY_OPTIONS ) );
	m_pCore          = pCore;
	m_IntValueLambda = nullptr;
/* TODO 
	{
		HWND hwndDlg = GetWindowHandle( );
		HWND hwndTip = CreateWindowEx
					  (
						  NULL, TOOLTIPS_CLASS, NULL,
				          WS_POPUP | TTS_NOPREFIX | TTS_ALWAYSTIP | TTS_BALLOON,
						  CW_USEDEFAULT, CW_USEDEFAULT,
						  CW_USEDEFAULT, CW_USEDEFAULT,
						  hwndDlg, NULL, GetModuleHandle( nullptr ),
						  NULL
					  );
		assert( hwndTip != nullptr );

		SetWindowPos
		(
			hwndTip, HWND_TOPMOST, 0, 0, 0, 0,
			SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE
		);

		PTSTR pszText = L"Fyxyxyxyxyx";

		// Associate the tooltip with the tool.
		TOOLINFO toolInfo = { 0 };
		toolInfo.cbSize = sizeof(toolInfo);
		toolInfo.hwnd = hwndDlg;
		toolInfo.uFlags = TTF_SUBCLASS | TTF_IDISHWND;
		toolInfo.uId = (UINT_PTR)hwndDlg;
		toolInfo.lpszText = pszText;
		::GetClientRect (hwndDlg, &toolInfo.rect);

		LRESULT lres = ::SendMessage( hwndTip, TTM_ADDTOOL, 0, (LPARAM)&toolInfo );
		assert( lres );
	}
*/
}

void DspOptWindow::UpdateDspOptionsControls( tBrushMode const brushMode )
{
	if ( brushMode != tBrushMode::move )
	{
		static unordered_map < tBrushMode, WORD > mapDspOptTable =
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

int DspOptWindow::getNeighborHoodMeanValue( GridPoint const & gp ) const
{ 
	int iSum = m_pCore->GetFoodStock( gp );
	Neighborhood::Apply2All
	( 
		gp, 
		[&](GridPoint const & gpNeighbor) 
	    { 
			iSum += m_pCore->GetFoodStock(gpNeighbor); 
		} 
	);
	return iSum / (Neighborhood::GetNrOfNeighbors( ) + 1);
};

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
				m_IntValueLambda = [&](GridPoint const & gp){ return m_pCore->GetMutRate( gp ); };
                break;

            case IDM_FERTILITY:
				m_IntValueLambda = [&](GridPoint const & gp){ return m_pCore->GetFertility( gp ); };
                break;

            case IDM_FOOD_STOCK:
				m_IntValueLambda = [&](GridPoint const & gp){ return getNeighborHoodMeanValue( gp ); };
                break;

            case IDM_FERTILIZER:
				m_IntValueLambda = [&](GridPoint const & gp){ return m_pCore->GetFertilizer( gp ); };
                break;

            case IDM_DSP_ENV_NOTHING:
				m_IntValueLambda = [&](GridPoint const & gp){ return 0; };
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
