// win32_displayOptions.cpp 
//

#include "stdafx.h"
#include <functional>
#include "Resource.h"
#include "gridNeighbor.h"
#include "EvolutionModelData.h"
#include "win32_util.h"
#include "win32_baseDialog.h"
#include "win32_worker_thread.h"
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
    WorkThread               * const pWorkThread,
    EvolutionModelData const * const pModel 
)
{
    StartBaseDialog( hWndParent, MAKEINTRESOURCE( IDD_DISPLAY_OPTIONS ) );
    m_pWorkThread    = pWorkThread;
	m_pModel         = pModel;
	m_IntValueLambda = nullptr;
}

void DspOptWindow::SetDisplayMode( WORD const wMode )
{ 
    HWND const hDlgItem = GetDlgItem( wMode );
    assert( hDlgItem != nullptr );

    (void)::SendMessage( hDlgItem, BM_CLICK, 0 , 0);
};

void DspOptWindow::SetIndividualsVisible( )
{
    (void)::SendMessage( GetDlgItem( IDM_ANIMALS ), BM_SETCHECK, BST_CHECKED, 0 );
}

BOOL DspOptWindow::AreIndividualsVisible( ) const
{
    return ( BST_CHECKED == ::SendMessage( GetDlgItem( IDM_ANIMALS ), BM_GETCHECK, 0, 0 ) );
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
        (void)::SendMessage( GetDlgItem( IDM_ANIMALS),    BM_SETCHECK, BST_CHECKED, 0 );
        (void)::SendMessage( GetDlgItem( IDM_FOOD_STOCK), BM_SETCHECK, BST_CHECKED, 0 );
        return static_cast<INT_PTR>(TRUE);

    case WM_COMMAND:
        {
            WORD const wId = LOWORD(wParam);
            switch ( wId )
            {
            case IDM_MOVE:
            case IDM_ANIMALS:
            case IDM_RANDOM_STRATEGY:
            case IDM_COOPERATE:
            case IDM_DEFECT:
            case IDM_TIT4TAT:
                break;

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
                Show( tBoolOp::opFalse );
                return static_cast<INT_PTR>(TRUE);

            default:
                break;
            }
            m_pWorkThread->PostRefresh( );
        }
        break;

    default:
        break;
    }

    return static_cast<INT_PTR>(FALSE);
}
