// NobId.ixx
//
// NNetModel

export module NNetModel:NobId;

import Types;

export using NobId = NamedType<short, struct NobIdParam>;

export inline NobId const NO_NOB(-1);

export inline bool IsDefined(NobId const id)
{
	return id != NO_NOB;
}

export inline bool IsUndefined(NobId const id)
{
	return id == NO_NOB;
}
