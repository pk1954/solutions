// ModelDescription.h : 
//
// NNetModel

#pragma once

#include <string>

using std::wstring;

class ModelDescription
{
public:
	virtual void ClearDescription( ) = 0;
	virtual void SetDescription( wstring const ) = 0;
	//virtual void AppendDescriptionLine( wstring const ) = 0;
	virtual bool GetDescriptionLine( int const, wstring & ) const = 0;
};
