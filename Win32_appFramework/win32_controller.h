// win32_controller.h
//
// Win32_appFramework

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

	bool ProcessFrameworkCommand( WPARAM const, LPARAM const = 0 );

	virtual void ProcessAppCommand( WPARAM const, LPARAM const = 0 ) = 0;

private:
	BaseWindow          * m_pAppWindow;
	WorkThreadInterface * m_pWorkThreadInterface;
};				          
