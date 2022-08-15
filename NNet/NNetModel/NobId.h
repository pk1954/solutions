// NobId.h
//
// NNetModel

#pragma once

import NamedType;

using NobId = NamedType<int, struct NobIdParam>;

inline NobId const NO_NOB(-1);

inline bool IsDefined(NobId const id) 
{ 
	return id != NO_NOB; 
}

inline bool IsUndefined(NobId const id) 
{ 
	return id == NO_NOB; 
}
