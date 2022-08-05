// DisplayFunctor.ixx
//
// Utilities

module;

#include <string>

export module DisplayFunctor;

using std::wstring;

export class DisplayFunctor
{
public:
	virtual ~DisplayFunctor() = default;
	virtual void operator() (wstring const &) = 0;
};
