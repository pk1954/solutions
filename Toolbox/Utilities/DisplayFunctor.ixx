// DisplayFunctor.h
//
// Utilities

module;

#pragma once

#include <string>

export module DisplayFunctor;

export class DisplayFunctor
{
public:
	virtual ~DisplayFunctor() = default;
	virtual void operator() (std::wstring const &) = 0;
};
