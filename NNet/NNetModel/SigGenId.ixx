// SigGenId.ixx
//
// NNetModel

export module SigGenId;

import NamedType;

export using SigGenId = NamedType<int, struct SigGenIdParam>;

export inline SigGenId const NO_SIGGEN(-1);

export bool IsDefined(SigGenId const id)
{
	return id != NO_SIGGEN;
}

export bool IsUndefined(SigGenId const id)
{
	return id == NO_SIGGEN;
}
