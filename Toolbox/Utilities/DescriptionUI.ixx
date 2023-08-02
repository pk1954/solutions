// DescriptionUI.ixx
//
// Toolbox\Utilities

module;

#include <string>

export module DescriptionUI;

using std::wstring;

export class DescriptionUI
{
public:
	virtual ~DescriptionUI() = default;
	virtual void ClearDescription()                             = 0;
	virtual void SetDescription(wstring const &)                = 0;
	virtual int  GetLineCount()                           const = 0;
	virtual bool GetDescriptionLine(int const, wstring &) const = 0;
	virtual bool IsDirty()                                      = 0;
	virtual void ResetDirtyFlag()                               = 0;
};
