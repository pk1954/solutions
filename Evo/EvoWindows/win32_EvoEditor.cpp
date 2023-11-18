// win32_EvoEditor.cpp 
//
// EvoWindows


#include <unordered_map>
#include "commctrl.h"
#include "Resource.h"
#include "EvolutionCore.h"
#include "EvoReadBuffer.h"
#include "win32_tooltip.h"
#include "win32_EvoWorkThreadInterface.h"
#include "win32_displayOptions.h"
#include "win32_EvoEditor.h"

import Config;

EvoEditorWindow::EvoEditorWindow()
  : BaseDialog(),
    m_pReadBuffer         (nullptr),
    m_pWorkThreadInterface(nullptr),
    m_pDspOptWindow       (nullptr)
{ }

void EvoEditorWindow::Start
( 
    HWND                     const hwndParent,
    EvoWorkThreadInterface * const pWorkThreadInterface,
	EvoReadBuffer          * const pReadBuffer,
    DspOptWindow           * const pDspOptWindow
)
{
    m_pWorkThreadInterface = pWorkThreadInterface;
    m_pReadBuffer          = pReadBuffer;
    m_pDspOptWindow        = pDspOptWindow;

	StartBaseDialog(hwndParent, MAKEINTRESOURCE(IDD_EDITOR), nullptr);  //[&](){ return ! m_pWorkThreadInterface->IsRunning(); });

    SetTrackBarRange(IDM_EDIT_SIZE,      1L,  50L);
    SetTrackBarRange(IDM_EDIT_INTENSITY, 0L, 100L);

	UpdateEditControls();

	CreateWindowToolTip(L"The editor allows to manipulate the model manually (individuals, mutation rate, fertility etc.) by using the left mouse button.");
	CreateBalloonToolTip(IDM_MOVE, L"Left mouse button moves the model on the screen (no changes to the model).");

	m_pReadBuffer->RegisterObserver(this);
}

void EvoEditorWindow::Stop()
{
	DestroyWindow();
	m_pWorkThreadInterface = nullptr;
	m_pReadBuffer          = nullptr;
	m_pDspOptWindow        = nullptr;
}

EvoEditorWindow::~EvoEditorWindow()
{
    m_pWorkThreadInterface = nullptr;
    m_pReadBuffer          = nullptr;
    m_pDspOptWindow        = nullptr;
}

LRESULT EvoEditorWindow::SendClick(int const item) const
{
	HWND    const hwndOld { SetActiveWindow(GetWindowHandle()) };
	LRESULT const res     { SendDlgItemMessage(item, BM_CLICK, 0, 0) };
    (void)SetActiveWindow(hwndOld);
    return res;
}

void EvoEditorWindow::updateOperationButtons(tBrushMode const mode) const
{
	bool bEnableOperationButtons = ! IsStrategyBrushMode(mode);

	EnableWindow(GetDlgItem(IDM_EDIT_OPERATION_SET),      bEnableOperationButtons);
	EnableWindow(GetDlgItem(IDM_EDIT_OPERATION_MIN),      bEnableOperationButtons);
	EnableWindow(GetDlgItem(IDM_EDIT_OPERATION_MAX),      bEnableOperationButtons);
	EnableWindow(GetDlgItem(IDM_EDIT_OPERATION_ADD),      bEnableOperationButtons);
	EnableWindow(GetDlgItem(IDM_EDIT_OPERATION_SUBTRACT), bEnableOperationButtons);
}

void EvoEditorWindow::UpdateEditControls() // Set state of all window widgets according to mode (edit/simu)
{
	EvolutionCore const * pCore = m_pReadBuffer->LockReadBuffer();

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
    
	CheckRadioButton(IDM_MOVE, IDM_FOOD_STOCK, mapModeTable.at(pCore->GetBrushMode()));

	static std::unordered_map < tShape, WORD > mapShapeTable
	{
		{ tShape::Circle, IDM_EDIT_CIRCLE    },    
		{ tShape::Rect,   IDM_EDIT_RECTANGLE },
		{ tShape::Grid,   IDM_EDIT_GRID_AREA }
	};

	CheckRadioButton(IDM_EDIT_CIRCLE, IDM_EDIT_GRID_AREA, mapShapeTable.at(pCore->GetBrushShape()));

	updateOperationButtons(pCore->GetBrushMode());

	static std::unordered_map < tManipulator, WORD > mapOperatorTable
	{
		{ tManipulator::set,      IDM_EDIT_OPERATION_SET      },    
		{ tManipulator::min,      IDM_EDIT_OPERATION_MIN      },
		{ tManipulator::max,      IDM_EDIT_OPERATION_MAX      },
		{ tManipulator::add,      IDM_EDIT_OPERATION_ADD      },
		{ tManipulator::subtract, IDM_EDIT_OPERATION_SUBTRACT },
	};

	CheckRadioButton(IDM_EDIT_OPERATION_SET, IDM_EDIT_OPERATION_SUBTRACT, mapOperatorTable.at(pCore->GetBrushManipulator()));
	
	SetTrackBarPos(IDM_EDIT_SIZE,      pCore->GetBrushSize().GetValue());
    SetTrackBarPos(IDM_EDIT_INTENSITY, pCore->GetBrushIntensity().GetValue());

	// adjust display options window

	m_pDspOptWindow->UpdateDspOptionsControls(pCore->GetBrushMode());
	m_pReadBuffer->ReleaseReadBuffer();
}

void EvoEditorWindow::setBrushMode(WORD const wId) const
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

	tBrushMode const brushMode { mapModeTable.at(wId) };
	m_pWorkThreadInterface->PostSetBrushMode(brushMode);
	m_pDspOptWindow->UpdateDspOptionsControls(brushMode);
}

void EvoEditorWindow::setBrushShape(WORD const wId) const
{
	static std::unordered_map < WORD, tShape > mapShapeTable
	{
		{ IDM_EDIT_CIRCLE,    tShape::Circle },    
		{ IDM_EDIT_RECTANGLE, tShape::Rect   },
		{ IDM_EDIT_GRID_AREA, tShape::Grid   }
	};

	tShape const brushShape { mapShapeTable.at(wId) };
	m_pWorkThreadInterface->PostSetBrushShape(brushShape);
}

void EvoEditorWindow::setBrushManipulator(WORD const wId) const
{
	static std::unordered_map < WORD, tManipulator > mapOperationTable
	{
		{ IDM_EDIT_OPERATION_SET,      tManipulator::set      },    
		{ IDM_EDIT_OPERATION_MIN,      tManipulator::min      },    
		{ IDM_EDIT_OPERATION_MAX,      tManipulator::max      },    
		{ IDM_EDIT_OPERATION_ADD,      tManipulator::add      },    
		{ IDM_EDIT_OPERATION_SUBTRACT, tManipulator::subtract },    
	};

	tManipulator const brushOperator { mapOperationTable.at(wId) };
	m_pWorkThreadInterface->PostSetBrushManipulator(brushOperator);
}

LRESULT EvoEditorWindow::UserProc(UINT const message, WPARAM const wParam, LPARAM const lParam)
{
    switch (message)
    {
    case WM_HSCROLL:
		{
			HWND  const hwndTrackBar { (HWND)lParam };
			int   const iCtrlId      { GetDlgCtrlID(hwndTrackBar) };
			short const sLogicalPos  { GetTrackBarPos(iCtrlId) };
			switch (iCtrlId)
			{
			case IDM_EDIT_INTENSITY:
				m_pWorkThreadInterface->PostSetBrushIntensity(PERCENT(sLogicalPos));
				break;
			case IDM_EDIT_SIZE:
				m_pWorkThreadInterface->PostSetBrushRadius(GridCoord(sLogicalPos));
				break;
			default:
				assert(false);
			}
		}
        return TRUE;

	case WM_PAINT:
		UpdateEditControls();
		break;

	case WM_COMMAND:
	{
		WORD const wId { LOWORD(wParam) };

            switch (wId)
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
				setBrushMode(wId);
				break;

            case IDM_EDIT_CIRCLE:
            case IDM_EDIT_RECTANGLE:
            case IDM_EDIT_GRID_AREA:
				setBrushShape(wId);
                break;

			case IDM_EDIT_OPERATION_SET:     
			case IDM_EDIT_OPERATION_MIN:
			case IDM_EDIT_OPERATION_MAX:     
			case IDM_EDIT_OPERATION_ADD:     
			case IDM_EDIT_OPERATION_SUBTRACT:
				setBrushManipulator(wId);
                break;

			case IDM_EDIT_UNDO:
			case IDM_EDIT_REDO:
				PostCommand2Application(wId, 0);
				break;

			default:
				assert(false);
                break;
            }
        }
        break;

	case WM_ACTIVATE:
		PostCommand2Application(IDD_EDITOR, TRUE);
		break;

	case WM_CLOSE:
		AnimateWindow(GetWindowHandle(), 200, AW_HIDE | AW_VER_POSITIVE);
		PostCommand2Application(IDD_EDITOR, FALSE);
		return TRUE; 

    default:
        break;
    }

    return FALSE;
}
