// strlist.h - manage negative list

#pragma once

#include <map>
#include <string.h>
#include "scanner.h"

class NegativeList
{
public:
	virtual ~NegativeList( ) {};

	bool    Add      ( unsigned long, wstring const & );
	bool    SetActive( unsigned long, bool );
	void    SetAllActive( bool );
	wstring Check( wstring const & ) const;

private:
	struct StrInfo
	{
		StrInfo( ) 
		{
			m_fActive  = false;
			m_wstrNeg  = nullptr;
		}

		StrInfo( bool const f, wstring const & wstr ) 
		{
			m_fActive  = f;
			m_wstrNeg  = wstr;
		}

		bool    m_fActive;
		wstring m_wstrNeg;
	};

	std::map< unsigned long, StrInfo > m_negSpecs;
};