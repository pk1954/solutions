// NobId.ixx
//
// NNetModel

export module NobId;

import NamedType;

export using NobId = NamedType<int, struct NobIdParam>;

export inline NobId const NO_NOB(-1);

export inline bool IsDefined(NobId const id)
{
	return id != NO_NOB;
}

export inline bool IsUndefined(NobId const id)
{
	return id == NO_NOB;
}
