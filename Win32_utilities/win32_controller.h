// win32_controller.h
//
// Win32_utilities

#pragma once

class BaseWindow;
class WorkThreadInterface;

class Controller
{
public:
	Controller( );

	virtual ~Controller( );

	void Initialize
	( 
		BaseWindow          * const,
		WorkThreadInterface * const
	);

	bool ProcessCommand( WPARAM const, LPARAM const = 0 );

private:
	BaseWindow          * m_pAppWindow;
	WorkThreadInterface * m_pWorkThreadInterface;
};				          
