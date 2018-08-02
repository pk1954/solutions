// win32_displayOptions.cpp 
//

#include "stdafx.h"
#include <unordered_map>
#include <functional>
#include "Resource.h"
#include "windowsx.h"
#include "gridNeighbor.h"
#include "EvolutionModelData.h"
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
    HWND               const         hWndParent, 
    EvolutionModelData const * const pModel 
)
{
    StartBaseDialog( hWndParent, MAKEINTRESOURCE( IDD_DISPLAY_OPTIONS ) );
	m_pModel         = pModel;
	m_IntValueLambda = nullptr;
}

void DspOptWindow::UpdateDspOptionsControls( tBrushMode const brushMode )
{
	if ( brushMode != tBrushMode::move )
	{
		static unordered_map < tBrushMode, WORD > mapDspOptTable =
		{
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
	int iSum = m_pModel->GetFoodStock( gp );
	Neighborhood::Apply2All
	( 
		gp, 
		[&](GridPoint const & gpNeighbor) 
	    { 
			iSum += m_pModel->GetFoodStock(gpNeighbor); 
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
        return static_cast<INT_PTR>(TRUE);

    case WM_COMMAND:
        {
            WORD const wId = LOWORD(wParam);
            switch ( wId )
            {
            case IDM_MUT_RATE:
				m_IntValueLambda = [&](GridPoint const & gp){ return m_pModel->GetMutationRate( gp ); };
                break;

            case IDM_FERTILITY:
				m_IntValueLambda = [&](GridPoint const & gp){ return m_pModel->GetFertility( gp ); };
                break;

            case IDM_FOOD_STOCK:
				m_IntValueLambda = [&](GridPoint const & gp){ return getNeighborHoodMeanValue( gp ); };
                break;

            case IDM_FERTILIZER:
				m_IntValueLambda = [&](GridPoint const & gp){ return m_pModel->GetFertilizer( gp ); };
                break;

            case IDM_DSP_ENV_NOTHING:
				m_IntValueLambda = [&](GridPoint const & gp){ return 0; };
                break;

            case IDCANCEL:
                Show( FALSE );
                return static_cast<INT_PTR>(TRUE);

            default:
                break;
            }
			PostCommand2Application( IDM_REFRESH, 0 );
        }
        break;

    default:
        break;
    }

    return static_cast<INT_PTR>(FALSE);
}
