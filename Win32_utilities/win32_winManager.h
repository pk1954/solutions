// win32_winManager.h : 
//

#pragma once

#include <string>
#include <unordered_map>
#include "win32_util.h"
#include "win32_baseWindow.h"
#include "win32_baseDialog.h"

class RootWindow;

class WinManager
{
public:
    WinManager( );
    virtual ~WinManager( ) { };

	void AddWindow( std::wstring const, UINT const, HWND, BOOL const, BOOL const );
	void AddWindow( std::wstring const, UINT const, BaseWindow const &, BOOL const, BOOL const );
	void AddWindow( std::wstring const, UINT const, BaseDialog const &, BOOL const, BOOL const );

	void RemoveWindow( UINT const id )
	{
		m_map.erase( id );
	}

	std::wstring const GetWindowName( UINT const id )  const // can throw out_of_range exception
    {
        return m_map.at( id ).m_wstr;
    }

	HWND const GetHWND( UINT const id )  const // can throw out_of_range exception
	{
		return m_map.at( id ).m_hwnd;
	}

	BaseWindow const * const GetBaseWindow( UINT const id )  const // can throw out_of_range exception
	{
		return m_map.at( id ).m_pBaseWindow;
	}

	INT const GetIdFromRootWindow( HWND const hwnd )
	{
		for ( auto pp : m_map )
			if ( pp.second.m_hwnd == hwnd )
				return pp.first; 

		return -1;
	}

    BOOL const IsMoveable( UINT const id )  const // can throw out_of_range exception
    {
        return m_map.at( id ).m_bTrackPosition;
    }

	BOOL const IsSizeable( UINT const id )  const // can throw out_of_range exception
	{
		return m_map.at( id ).m_bTrackSize;
	}

	BOOL const IsVisible( UINT const id )  const // can throw out_of_range exception
	{
		return IsWindowVisible( GetHWND( id ) );
	}

	void Show( UINT const id, tBoolOp const op ) const
    {
        Util::Show( GetHWND( id ), op );
    }

    void SetWindowConfigurationFile( std::wstring const fileName ) 
    { 
        m_strWindowConfigurationFile = fileName; 
    };

    void IncNrOfMonitorConfigurations( ) 
    { 
        ++m_iNrOfMonitorConfigurations; 
    };

    BOOL GetWindowConfiguration( );
    void StoreWindowConfiguration( );

private:
    std::wstring const MONITOR_CONFIG_FILE     = L"MonitorConfigurations.cnf";
    std::wstring const WINDOW_CONFIG_FILE_STUB = L"WindowConfiguration";

    struct MAP_ELEMENT
    {
        std::wstring const   m_wstr;
		BaseWindow   const * m_pBaseWindow;    // Normally BaseWindows are managed, but in same cases
		HWND         const   m_hwnd;           // also naked HWNDs are used
		BOOL         const   m_bTrackPosition; // if TRUE, winManager sets window position from config file
		BOOL         const   m_bTrackSize;     // if TRUE, winManager sets window size from config file
    };

    std::unordered_map< UINT, MAP_ELEMENT > m_map;
    
    std::wstring m_strWindowConfigurationFile;
    int          m_iNrOfMonitorConfigurations;

    void dumpMonitorConfiguration( ) const;
    void dumpWindowCoordinates( ) const;
};
