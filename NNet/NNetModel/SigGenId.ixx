// SigGenId.ixx
//
// NNetModel

module;

#include <limits>

export module NNetModel:SigGenId;

import Types;

export using SigGenId = NamedType<int, struct SigGenIdParam>;

export inline SigGenId const STD_SIGGEN(-1);
export inline SigGenId const NO_SIGGEN (-2);
export inline SigGenId const ADD_SIGGEN(-3);

export inline bool IsValidSigGenId  (SigGenId const id) { return id >= STD_SIGGEN; }
export inline bool IsMutable        (SigGenId const id) { return id > STD_SIGGEN; }
export inline bool IsAddSigGenButton(SigGenId const id) { return id == ADD_SIGGEN; }
