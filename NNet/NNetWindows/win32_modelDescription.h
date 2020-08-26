// win32_modelDescription.h : 
//
// NNetWindows

#pragma once

#include <string>
#include "ModelDescription.h"
#include "win32_baseWindow.h"

class MessagePump;

using std::wstring;

class DescriptionWindow : public BaseWindow, public ModelDescription
{
public:
	void Start( HWND const, MessagePump & );
	void Stop( );

	virtual void ClearDescription( );
	virtual void SetDescription( wstring const );
	virtual bool GetDescriptionLine( int const, wstring & ) const;

private:
	HWND m_hwndEdit { nullptr };

	virtual LRESULT UserProc( UINT const, WPARAM const, LPARAM const );
};
