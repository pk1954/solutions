// DisplayFunctor.h
//
// Utilities

#pragma once

#include <string>

class DisplayFunctor
{
public:
	virtual ~DisplayFunctor() {};
	virtual void operator() (std::wstring const &) = 0;
};
