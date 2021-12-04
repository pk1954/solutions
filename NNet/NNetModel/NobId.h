// NobId.h
//
// NNetModel

#pragma once

#include "NamedType.h"

using NobId = NamedType<int, struct NobIdParam>;

inline NobId const NO_NOB(-1);

static bool IsDefined(NobId const id) 
{ 
	return id != NO_NOB; 
}

static bool IsUndefined(NobId const id) 
{ 
	return id == NO_NOB; 
}
