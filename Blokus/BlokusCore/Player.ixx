// Player.ixx
//
// BlokusCore

export module BlokusCore:Player;

import std;
import Types;
import Color;
import Direct2D;
import DrawContext;
import HiResTimer;
import :Board;
import :BoardMap;
import :Piece;
import :PlayerId;
import :PlayerTypes;
import :PieceTypeId;
import :RuleServerInterface;
import :Components;
import :BlokusCoords;
import :Strategy;
import :Move;

using std::array;
using std::vector;
using std::wstring;

export class Player
{
public:
    void Reset(PlayerType const& type, Strategy * const);

    void ResetTimer() { m_timer.Reset(); }

    bool IsFirstMove() const { return m_bFirstMove; }

    wstring const &GetName() const { return m_pPlayerType->m_wstrName; }

    Piece const& GetPieceC(PieceTypeId const id)  { return m_pieces.at(id.GetValue()); }
    Piece      & GetPiece (PieceTypeId const id)  { return m_pieces.at(id.GetValue()); }

    void DrawFreePieces (DrawContext&) const;
    void DrawContactPnts(DrawContext&) const;
    void DrawResult     (DrawContext&, TextFormatHandle const) const;
    void DrawCell       (DrawContext&, CoordPos const&) const;

    Move SelectMove(RuleServerInterface const&);
    void PerformMove(Move const&);

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

    void Apply2AvailablePieces(auto const& func)
    {
        for (Piece &piece: m_pieces)
            if (piece.IsAvailable())
                func(piece);
    }

    void Apply2AvailablePiecesC(auto const& func) const
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

    void ClearContactPnts()
    {
        m_contactPntsOnBoard.clear();
    }

    void AddContactPnt(CoordPos const &pos)
    {
        m_contactPntsOnBoard.push_back(pos);
    }

    void AddInitialContactPnt()
    {
        AddContactPnt(m_pPlayerType->m_startPoint);
    }

    bool IsValidPos(CoordPos const &pos) const 
    { 
        return m_validPositions.IsValidPos(pos); 
    }

    bool            HasFinished() const { return m_bFinished; }
    int             Result()      const { return m_iResult; }
    Ticks           GetTicks()    const { return m_timer.GetAccumulatedActionTicks(); }
    Strategy const &GetStrategy() const { return *m_pStrategy; }

private:
    using PIECE_TYPE_SET = array<Piece, NR_OF_PIECE_TYPES>;

    PlayerType const * m_pPlayerType;
    Strategy         * m_pStrategy;
    unsigned int       m_remainingPieces; 
    int                m_iResult;
    bool               m_bFinished;       
    bool               m_bFirstMove;      
    PIECE_TYPE_SET     m_pieces;
    vector<CoordPos>   m_contactPntsOnBoard;
    BoardMap           m_validPositions;
    HiResTimer         m_timer;

    void reduceValidMoves(Move const&);
    void finalize        (Move const&);
};