// win32_editor.h 
//
// NNetWindows

#pragma once

#include "NNetReadBuffer.h"
#include "win32_baseDialog.h"

class NNetWorkThreadInterface;

class NNetEditorWindow : public BaseDialog
{
public:
	NNetEditorWindow( );
	~NNetEditorWindow( );

	void Start
	( 
		HWND const, 
		NNetWorkThreadInterface * const, 
		NNetReadBuffer          * const 
	);

	void Stop( );

	void    UpdateEditControls( );
	LRESULT SendClick( int ) const;

private:
	virtual LRESULT UserProc( UINT const, WPARAM const, LPARAM const );

	NNetReadBuffer          * m_pReadBuffer;
	NNetWorkThreadInterface * m_pWorkThreadInterface;
};
