// DescriptionUI.h : 
//
// NNetModel

#pragma once

#include <string>

using std::wstring;

class DescriptionUI
{
public:
	virtual void       ClearDescription() = 0;
	virtual void       SetDescription( wstring const ) = 0;
	virtual int  const GetLineCount() const = 0;;
	virtual bool const GetDescriptionLine( int const, wstring & ) const = 0;
	virtual bool const IsDirty() = 0;
	virtual void       ResetDirtyFlag() = 0;
};
