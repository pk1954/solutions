// win32_controller.cpp
//
// Win32_appFramework

#include "stdafx.h"
#include "win32_aboutBox.h"
#include "win32_baseWindow.h"
#include "win32_util_resource.h"
#include "win32_WorkThreadInterface.h"
#include "win32_controller.h"

Controller::Controller() :
	m_pWorkThreadInterface( nullptr ),
	m_pAppWindow          ( nullptr )
{ }					    

Controller::~Controller( )
{
	m_pWorkThreadInterface = nullptr;
	m_pAppWindow           = nullptr;
}

void Controller::Initialize
( 
	BaseWindow          * const pAppwindow,
	WorkThreadInterface * const pWorkThreadInterface
)
{
	m_pWorkThreadInterface = pWorkThreadInterface;
	m_pAppWindow           = pAppwindow;
}

bool Controller::ProcessCommand( WPARAM const wParam, LPARAM const lParam )
{
	int const wmId = LOWORD( wParam );

	switch (wmId)
	{
	case IDM_ABOUT:
		ShowAboutBox( m_pAppWindow->GetWindowHandle( ) );
		break;

	case IDM_EXIT:
		PostMessage( m_pAppWindow->GetWindowHandle( ), WM_CLOSE, 0, 0 );
		break;

	case IDM_FORWARD:
		m_pWorkThreadInterface->PostGenerationStep( );
		break;

	case IDM_BACKWARDS:
		m_pWorkThreadInterface->PostPrevGeneration( );
		break;

	case IDM_EDIT_UNDO:
		m_pWorkThreadInterface->PostUndo( );
		break;

	case IDM_EDIT_REDO:
		m_pWorkThreadInterface->PostRedo( );
		break;

	case IDM_RUN:
		m_pWorkThreadInterface->PostRunGenerations( true );
		break;

	case IDM_STOP:
		m_pWorkThreadInterface->PostStopComputation( );
		break;

	case IDM_HIST_BUFFER_FULL:
		std::wcout << L"History buffer is full" << std::endl;
		(void)MessageBeep( MB_ICONWARNING );
		ProcessCommand( IDM_STOP );
		break;

	default:
		return FALSE; // command has not been processed
	}

	return TRUE;  // command has been processed
}
