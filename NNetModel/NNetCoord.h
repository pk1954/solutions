// NNetCoord.h
//
// NNetModel

#pragma once

#include "util.h"
#include "NamedType.h"

using nm = NamedType< double, struct NNET_Parameter >;

constexpr const nm operator"" _nm( const long double d )
{
	return nm( d );
}

nm const MAX_NM{ 1.0e10_nm };
