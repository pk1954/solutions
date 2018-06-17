// win32_displayOptions.cpp 
//

#include "stdafx.h"
#include "Resource.h"
#include "gridNeighbor.h"
#include "EvolutionModelData.h"
#include "win32_util.h"
#include "win32_baseDialog.h"
#include "win32_worker_thread.h"
#include "win32_displayOptions.h"

// The family of GetIntValueFunctor classes provides functors for access to 
// EvolutionCore functions of return type int

class GetEmptyFunctor : public GetIntValueFunctor
{
    public:
        GetEmptyFunctor( EvolutionModelData const * pModel ) :
            GetIntValueFunctor( pModel )
        { }

        //lint -esym( 715, gp )    symbol not referenced
        virtual int operator() ( GridPoint const & gp ) const { return 0; }
};

class GetFoodStockFunctor : public GetIntValueFunctor
{
public: 
    GetFoodStockFunctor( EvolutionModelData const * pModel ) :
        GetIntValueFunctor( pModel )
    { }

    virtual int operator() ( GridPoint const & gp ) const 
    {
        int iSum = m_pModelWork->GetFoodStock( gp );
        Neighborhood::Apply2All
		( 
			gp, 
			[&](GridPoint const & gpNeighbor) 
			{
				iSum += m_pModelWork->GetFoodStock( gpNeighbor );
			}
		);
        return iSum / (Neighborhood::GetNrOfNeighbors( ) + 1);
    }
};

class GetFertilityFunctor : public GetIntValueFunctor
{
    public: 
        GetFertilityFunctor( EvolutionModelData const * pModel ) :
            GetIntValueFunctor( pModel )
        { }

        virtual int operator() ( GridPoint const & gp ) const { return m_pModelWork->GetFertility( gp ); }
};

class GetMutationRateFunctor : public GetIntValueFunctor
{
    public: 
        GetMutationRateFunctor( EvolutionModelData const * pModel ) :
            GetIntValueFunctor( pModel )
        { }

        virtual int operator() ( GridPoint const & gp ) const { return m_pModelWork->GetMutationRate( gp ); }
};
    
class GetFertilizerFunctor : public GetIntValueFunctor
{
    public: 
        GetFertilizerFunctor( EvolutionModelData const * pModel ) :
            GetIntValueFunctor( pModel )
        { }

        virtual int operator() ( GridPoint const & gp ) const { return m_pModelWork->GetFertilizer( gp ); }
};
    
//// end of GetIntValueFunctor classes //////////////////////////////

DspOptWindow::DspOptWindow(  )
    : BaseDialog( )
{ }

DspOptWindow::~DspOptWindow( )
{
    delete m_pGetFoodStockFunctor;
    delete m_pGetFertilityFunctor;
    delete m_pGetMutationRateFunctor;
    delete m_pGetFertilizerFunctor;
    delete m_pGetEmptyFunctor;
    m_pGetIntValueFunctor = nullptr;
}

void DspOptWindow::Start
( 
    HWND         const         hWndParent, 
    WorkThread * const         pWorkThread,
    EvolutionModelData    const * const pModel 
)
{
    StartBaseDialog( hWndParent, MAKEINTRESOURCE( IDD_DISPLAY_OPTIONS ) );
    m_pWorkThread             = pWorkThread;
    m_pGetFoodStockFunctor    = new GetFoodStockFunctor( pModel );
    m_pGetFertilityFunctor    = new GetFertilityFunctor( pModel );
    m_pGetMutationRateFunctor = new GetMutationRateFunctor( pModel );
    m_pGetFertilizerFunctor   = new GetFertilizerFunctor( pModel );
    m_pGetEmptyFunctor        = new GetEmptyFunctor( pModel );
    m_pGetIntValueFunctor     = m_pGetFoodStockFunctor;
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
                m_pGetIntValueFunctor = m_pGetMutationRateFunctor;
                break;

            case IDM_FERTILITY:
                m_pGetIntValueFunctor = m_pGetFertilityFunctor;
                break;

            case IDM_FOOD_STOCK:
                m_pGetIntValueFunctor = m_pGetFoodStockFunctor;
                break;

            case IDM_FERTILIZER:
                m_pGetIntValueFunctor = m_pGetFertilizerFunctor;
                break;

            case IDM_DSP_ENV_NOTHING:
                m_pGetIntValueFunctor = m_pGetEmptyFunctor;
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
