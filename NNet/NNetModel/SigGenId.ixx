// SigGenId.ixx
//
// NNetModel

export module NNetModel:SigGenId;

import Types;

export using SigGenId = NamedType<int, struct SigGenIdParam>;

export inline SigGenId const STD_SIGGEN(-1);
