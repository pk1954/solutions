// DspOptWindow.cpp 
//
// EvoWindows

module;

#include "windowsx.h"

module DspOptWindow;

import std;
import GridNeighborhood;
import Resource;
import EvolutionCore;
import Tooltip;
import BaseDialog;
import WinBasics;

DspOptWindow::DspOptWindow() : 
	BaseDialog()
{ }

DspOptWindow::~DspOptWindow()
{
	m_IntValueLambda = nullptr;
}

void DspOptWindow::Start(HWND const hwndParent)
{
	HWND hwndDlg = StartBaseDialog(hwndParent, MAKEINTRESOURCE(IDD_DISPLAY_OPTIONS), nullptr);
	m_IntValueLambda = nullptr;
	SetIndividualsVisible();
	SetDisplayMode(IDM_DSP_ENV_NOTHING);

	CreateWindowToolTip(L"Display options allow to show/hide individuals and to choose what is displayed as background.");
	CreateBalloonToolTip(IDM_INDIVIDUALS,     L"if deselected, individuals are hidden. Rarelay used.");
	CreateBalloonToolTip(IDM_MUT_RATE,        L"Show mutation rate as background.");
	CreateBalloonToolTip(IDM_MUT_RATE,        L"Show mutation rate as background.");
	CreateBalloonToolTip(IDM_FERTILITY ,      L"Show fertility as background.");
	CreateBalloonToolTip(IDM_FOOD_STOCK,      L"Show available food as background.");
	CreateBalloonToolTip(IDM_FERTILIZER,      L"Show amount of fertilizer as background.");
	CreateBalloonToolTip(IDM_DSP_ENV_NOTHING, L"Show no background.");
}

void DspOptWindow::Stop()
{
	DestroyWindow();
}

void DspOptWindow::UpdateDspOptionsControls(tBrushMode const brushMode)
{
	if (brushMode != tBrushMode::move)
	{
		static std::unordered_map < tBrushMode, WORD > mapDspOptTable =
		{
			{ tBrushMode::randomStrat, IDM_INDIVIDUALS },
			{ tBrushMode::cooperate,   IDM_INDIVIDUALS },
			{ tBrushMode::defect,      IDM_INDIVIDUALS },
			{ tBrushMode::tit4tat,     IDM_INDIVIDUALS },
			{ tBrushMode::noAnimals,   IDM_INDIVIDUALS },
			{ tBrushMode::mutRate,     IDM_MUT_RATE    },
			{ tBrushMode::fertility,   IDM_FERTILITY   },
			{ tBrushMode::food,        IDM_FOOD_STOCK  },
			{ tBrushMode::fertilizer,  IDM_FERTILIZER  }
		};
		WORD const wDspOptId = mapDspOptTable.at(brushMode);
		if (wDspOptId == IDM_INDIVIDUALS)
			SetIndividualsVisible();
		else 
			SetDisplayMode(wDspOptId);
	}
}

void DspOptWindow::SetDisplayMode(WORD const wMode)
{ 
    HWND const hDlgItem = GetDlgItem(wMode);
    Assert(hDlgItem != nullptr);

    (void)::SendMessage(hDlgItem, BM_CLICK, 0 , 0);
};

void DspOptWindow::SetIndividualsVisible()
{
	Button_SetCheck(GetDlgItem(IDM_INDIVIDUALS), BST_CHECKED);
}

bool DspOptWindow::AreIndividualsVisible() const
{
    return (BST_CHECKED == Button_GetCheck(GetDlgItem(IDM_INDIVIDUALS)));
}

LRESULT DspOptWindow::UserProc(UINT const message, WPARAM const wParam, LPARAM const lParam)
{
    switch (message)
    {
    case WM_INITDIALOG:
		Button_SetCheck(GetDlgItem(IDM_INDIVIDUALS), BST_CHECKED);
		Button_SetCheck(GetDlgItem(IDM_FOOD_STOCK ), BST_CHECKED);
        return TRUE;

    case WM_COMMAND:
        {
            WORD const wId = LOWORD(wParam);
            switch (wId)
            {
            case IDM_MUT_RATE:
				m_IntValueLambda = [&](EvolutionCore const * const pCore, GridPoint const gp){ return pCore->GetMutRate(gp).GetValue(); };
                break;

            case IDM_FERTILITY:
				m_IntValueLambda = [&](EvolutionCore const * const pCore, GridPoint const gp){ return pCore->GetFertility(gp).GetValue(); };
                break;

            case IDM_FOOD_STOCK:
				m_IntValueLambda = [&](EvolutionCore const * const pCore, GridPoint const gp){ return pCore->GetFoodStock(gp).GetValue(); };
                break;

            case IDM_FERTILIZER:
				m_IntValueLambda = [&](EvolutionCore const * const pCore, GridPoint const gp){ return pCore->GetFertilizer(gp).GetValue(); };
                break;

            case IDM_DSP_ENV_NOTHING:
				m_IntValueLambda = [&](EvolutionCore const * const pCore, GridPoint const gp){ return 0; };
                break;

            case IDCANCEL:
                Show(FALSE);
                return TRUE;

            default:
                break;
            }
			PostCommand2Application(IDM_REFRESH, 0);
        }
        break;

    default:
        break;
    }

    return FALSE;
}
