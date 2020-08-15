// win32_NNetWindow.cpp
//
// NNetWindows

#include "stdafx.h"
#include <sstream> 
#include "MoreTypes.h"
#include "Segment.h"
#include "scale.h"
#include "Pipe.h"
#include "InputNeuron.h"
#include "PixelTypes.h"
#include "NNetParameters.h"
#include "NNetModelReaderInterface.h"
#include "NNetColors.h"
#include "DrawModel.h"
#include "win32_sound.h"
#include "win32_tooltip.h"
#include "NNetModelWriterInterface.h"
#include "win32_NNetWindow.h"

using std::function;

void NNetWindow::InitClass( ActionTimer * const pActionTimer )
{
	ModelWindow::InitClass( pActionTimer );
}

NNetWindow::NNetWindow( ) :
	ModelWindow( )
{ }

void NNetWindow::Start
( 
	HWND                       const hwndApp, 
	DWORD                      const dwStyle,
	bool                       const bShowRefreshRateDialog,
	NNetController           * const pController,
	NNetModelReaderInterface * const pModelReaderInterface,
	DrawModel                * const pDrawModel,
	Observable               * const pCursorObservable
)
{
	m_pDrawModel = pDrawModel;
	HWND hwnd = StartBaseWindow
	( 
		hwndApp,
		CS_OWNDC | CS_DBLCLKS,
		L"ClassNNetWindow",
		dwStyle,
		nullptr,
		nullptr
	);
	m_context.Start( hwnd );
	m_pController           = pController;
	m_pModelReaderInterface = pModelReaderInterface;
	ShowRefreshRateDlg( bShowRefreshRateDialog );
}

void NNetWindow::Stop( )
{
	m_context.Stop();
	DestroyWindow( );
}

NNetWindow::~NNetWindow( )
{
	m_pModelReaderInterface = nullptr;
	m_pDrawModel            = nullptr;
	m_pController           = nullptr;
}

MicroMeterRect const NNetWindow::GetEnclosingRect() const 
{ 
	return m_pModelReaderInterface->GetEnclosingRect(); 
}

MicroMeterRect const NNetWindow::GetViewRect() const 
{ 
	return m_context.GetCoordC().Convert2MicroMeterRect( GetClPixelRect() ); 
};

void NNetWindow::OnPaint( )
{
	if ( IsWindowVisible() )
	{
		PAINTSTRUCT ps;
		HDC const hDC = BeginPaint( &ps );
		if ( m_context.StartFrame( hDC ) )
		{
			doPaint( );
			m_context.EndFrame( );
		}
		EndPaint( &ps );
	}
}

void NNetWindow::OnSize( WPARAM const wParam, LPARAM const lParam )
{
	UINT width  = LOWORD(lParam);
	UINT height = HIWORD(lParam);
	m_context.Resize( width, height );
	Notify( false );
}

bool NNetWindow::OnCommand( WPARAM const wParam, LPARAM const lParam, PixelPoint const pixPoint )
{
	MicroMeterPoint const umPoint { m_context.GetCoordC().Convert2MicroMeterPointPos( pixPoint ) };
	if ( m_pController->HandleCommand( LOWORD( wParam ), lParam, umPoint ) )
		return true;
	else 
		return ModelWindow::OnCommand( wParam, lParam, pixPoint );
}
