// ModelDescription.h : 
//
// NNetModel

#pragma once

#include <vector>
#include <string>

using std::vector;
using std::wstring;

class ModelDescription
{
public:
	bool operator==( ModelDescription const& rhs ) const
	{
		if ( m_lines.size() != rhs.m_lines.size()  )
			return false;
		for ( int i = 0; i < m_lines.size(); ++i )
			if ( m_lines[i] != rhs.m_lines[i] )
				return false;
		return true;
	}

	void ClearDescription( )
	{
		m_lines.clear();
	}

	void AddDescriptionLine( wstring const wstrLine )
	{
		m_lines.push_back( wstrLine + L"\r\n" );
	}

	bool GetDescriptionLine( unsigned int const uiLineNr, wstring & wstrLine ) const
	{
		if ( uiLineNr >= m_lines.size() )
			return false;

		wstrLine = m_lines[ uiLineNr ];
	    return true;;
	}

private:
	vector<wstring> m_lines;
};
