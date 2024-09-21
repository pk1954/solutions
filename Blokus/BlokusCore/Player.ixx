// Player.ixx
//
// BlokusCore

export module BlokusCore:Player;

import std;
import Types;
import Color;
import Random;
import Direct2D;
import :Piece;
import :PlayerId;
import :PieceTypeId;
import :Components;
import :CoordPos;
import :Move;

using std::array;
using std::vector;

export class Player
{
public:
    void Initialize
    (
        CoordPos const startPoint,
        Color    const col
    )
    {
        PieceTypeId id { 0 };
        Apply2AllPieces([&id](Piece& p){ p.Initialize(id++); });
        m_color = col;
        m_contactPntsOnBoard.push_back(startPoint);
    }

    Piece const& GetPieceC(PieceTypeId const id)  { return m_pieces.at(id.GetValue()); }
    Piece      & GetPiece (PieceTypeId const id)  { return m_pieces.at(id.GetValue()); }

    void DrawFreePieces (D2D_driver const&, BlokusCoordSys const&) const;
    void DrawContactPnts(D2D_driver const&, BlokusCoordSys const&) const;
    void DrawCell       (D2D_driver const&, BlokusCoordSys const&, CoordPos const&) const;

    void Apply2AllPieces(auto const& func)
    {
        for (Piece& piece: m_pieces)
            func(piece);
    }

    void Apply2AllPiecesC(auto const& func) const
    {
        for (Piece const& piece: m_pieces)
            func(piece);
    }

    void Apply2FreePiecesC(auto const& func) const
    {
        for (Piece const& piece: m_pieces)
            if (piece.IsAvailable())
                func(piece);
    }

    void Apply2AllContactPntsC(auto const& func) const
    {
        for (CoordPos const& pos : m_contactPntsOnBoard)
            func(pos);
    }

    Move const& SelectMove(vector<Move> const& moves) const
    {
        //unsigned int uiMax { Cast2UnsignedInt(moves.size()) - 1 };
        //unsigned int uiSel { m_random.NextRandomNumberScaledTo(uiMax) };
        //return moves[uiSel];
        return moves[0];
    }

    void PerformMove(Move const& move)
    {
        PieceType const &pieceType { Components::GetPieceTypeC(move.m_idPieceType) };
        Shape     const &shape     { pieceType.GetShapeC(move.m_idShape)};
        Piece           &piece     { GetPiece(move.m_idPieceType) };
        piece.PerformMove(move);
        shape.Apply2AllContactPntsC
        (
            [this, &move](ShapeCoordPos const &shapePos)
            { 
                CoordPos const coordPos { move.m_boardPos + shapePos };
                if (IsOnBoard(coordPos))
                    m_contactPntsOnBoard.push_back(coordPos); 
            }
        );
    }

private:
   // static Random m_random;

    Color m_color;

    array<Piece, NR_OF_PIECE_TYPES> m_pieces;

    vector<CoordPos> m_contactPntsOnBoard;

    fPixelPoint getCenter(BlokusCoordSys const&, CoordPos const&) const;
};