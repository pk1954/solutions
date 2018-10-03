// win32_winManager.h : 
//

#pragma once

#include <string>
#include <unordered_map>
#include "win32_util.h"

using namespace std;

class RootWindow;

class WinManager
{
public:
    WinManager( );
    virtual ~WinManager( ) { };

    void AddWindow( wstring const, UINT const, RootWindow const *, BOOL const, BOOL const );

    wstring const GetWindowName( UINT const id )  const // can throw out_of_range exception
    {
        return m_map.at( id ).m_wstr;
    }

    HWND const GetHWND( UINT const id )  const // can throw out_of_range exception
    {
        return getWindowHandle( m_map.at( id ) );
    }

	INT const GetIdFromRootWindow( RootWindow const * pRootWin )
	{
		for ( auto pp : m_map )
			if ( pp.second.m_pRootWin == pRootWin )
				return pp.first; 

		return -1;
	}

    RootWindow const * const GetRootWindow( UINT const id )  const // can throw out_of_range exception
    {
        return m_map.at( id ).m_pRootWin;
    }

    BOOL const IsMoveable( UINT const id )  const // can throw out_of_range exception
    {
        return m_map.at( id ).m_bTrackPosition;
    }

    BOOL const IsSizeable( UINT const id )  const // can throw out_of_range exception
    {
        return m_map.at( id ).m_bTrackSize;
    }

    void Show( UINT const id, tBoolOp const op ) const
    {
        Util::Show( GetHWND( id ), op );
    }

    void SetWindowConfigurationFile( wstring const fileName ) 
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
    wstring const MONITOR_CONFIG_FILE     = L"MonitorConfigurations.cnf";
    wstring const WINDOW_CONFIG_FILE_STUB = L"WindowConfiguration";

    struct MAP_ELEMENT
    {
        wstring    const   m_wstr;
        RootWindow const * m_pRootWin;
		BOOL       const   m_bTrackPosition; // if TRUE, winManager sets window position from config file
		BOOL       const   m_bTrackSize;     // if TRUE, winManager sets window size from config file
    };

    unordered_map< UINT, MAP_ELEMENT > m_map;
    
    wstring m_strWindowConfigurationFile;
    int     m_iNrOfMonitorConfigurations;

    void       dumpMonitorConfiguration( ) const;
    void       dumpWindowCoordinates( ) const;
    HWND const getWindowHandle( MAP_ELEMENT const & elem ) const;
};
