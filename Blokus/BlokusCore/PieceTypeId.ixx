// BlokusCore.ixx
//
// PieceTypeId

export module BlokusCore:PieceTypeId;

import Types;

export int const UndefinedPieceTypeId { 255 };
export int const NR_OF_PIECE_TYPES    { 21 };

export using PieceTypeId = NamedType<unsigned char, struct PieceTypeId_Parameter>;

export bool IsValidPiecTypeId(PieceTypeId const id)
{
    return id.GetValue() < NR_OF_PIECE_TYPES;
}

export void Apply2AllPieceTypes(auto const& func)
{
    for (int i = 0; i < NR_OF_PIECE_TYPES; ++i)
        func(PieceTypeId(i));
}