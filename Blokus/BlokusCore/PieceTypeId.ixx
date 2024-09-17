// BlokusCore.ixx
//
// PieceTypeId

export module BlokusCore:PieceTypeId;

import Types;

export using PieceTypeId = NamedType<unsigned char, struct PieceTypeId_Parameter>;

export int const NR_OF_PIECE_TYPES { 21 };

export bool IsValidPiecTypeId(PieceTypeId const id)
{
    return id.GetValue() < NR_OF_PIECE_TYPES;
}

