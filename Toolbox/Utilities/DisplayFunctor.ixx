// DisplayFunctor.ixx
//
// Utilities

export module DisplayFunctor;

import std;

using std::wstring;

export class DisplayFunctor
{
public:
	virtual ~DisplayFunctor() = default;
	virtual void operator() (wstring const &) = 0;
};
