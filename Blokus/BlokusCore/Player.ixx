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
import :MapOfValidCells;
import :Piece;
import :PlayerId;
import :PieceTypeId;
import :RuleServerInterface;
import :Components;
import :BlokusCoords;
import :Strategy;
import :BlokusMove;

using std::array;
using std::vector;
using std::wstring;

export class Player
{
public:
    void Initialize(Board const&, PlayerId const, Strategy * const);
    void Reset();
    void ResetTimer() { m_timer.Reset(); }
    bool IsFirstMove() const { return m_bFirstMove; }

    wstring const &GetName() const { return m_pPlayerType->m_wstrName; }

    Piece const& GetPieceC(PieceTypeId const id) const { return m_pieces.at(id.GetValue()); }
    Piece      & GetPiece (PieceTypeId const id)       { return m_pieces.at(id.GetValue()); }

    void DrawFreePieces (DrawContext&, Piece const * const, TextFormatHandle const)             const;
    void DrawContactPnts(DrawContext&)                                                          const;
    void DrawResult     (DrawContext&, TextFormatHandle const)                                  const;
    void DrawCell       (DrawContext&, CoordPos const&, wstring const&, TextFormatHandle const) const;

    BlokusMove SelectMove(RuleServerInterface const&) const;
    void       DoMove  (BlokusMove&);
    void       UndoMove();
    void       Prepare();
    void       Finalize();

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

    Piece const *FindPiece(auto const& crit) const // returns first piece satisfying crit
    {
        for (Piece const &piece: m_pieces)
            if (piece.IsAvailable() && crit(piece))
                return &piece;
        return nullptr;
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

    void Apply2SetPiecesC(auto const& func) const
    {
        for (Piece const& piece: m_pieces)
            if (!piece.IsAvailable())
                func(piece);
    }

    void Apply2AllContactPntsC(auto const& func) const
    {
        for (CoordPos const& pos : m_contactPntsOnBoard)
            func(pos);
    }

    bool IsTrue4AnyContactPnt(auto const& func) const
    {
        for (CoordPos const& pos : m_contactPntsOnBoard)
            if (func(pos))
                return true;
        return false;
    }

    bool AnyShapeCellsBlocked(BlokusMove const&) const;

    bool IsBlocked(CoordPos const &pos) const { return m_mapOfValidCells.IsBlocked(pos); }

    bool            HasFinished() const { return m_bFinished; }
    int             Result()      const { return m_iResult; }
    Ticks           GetTicks()    const { return m_timer.GetAccumulatedActionTicks(); }
    Strategy const &GetStrategy() const { return *m_pStrategy; }
    Color           GetColor()    const { return m_pPlayerType->m_color; }
    bool            IsHuman()     const { return GetStrategy().IsHuman(); }

    ListOfMoves const &GetListOfValidMoves() const { return m_validMoves; }

    void CheckListOfValidMoves();

private:
    using PIECE_TYPE_SET = array<Piece, NR_OF_PIECE_TYPES>;

    Board      const * m_pBoard;
    PlayerId           m_idPlayer;
    PlayerType const * m_pPlayerType;
    Strategy         * m_pStrategy;
    unsigned int       m_remainingPieces; 
    int                m_iResult;
    bool               m_bFinished;       
    bool               m_bFirstMove;      
    PIECE_TYPE_SET     m_pieces;

    vector<CoordPos>   m_contactPntsOnBoard;
    MapOfValidCells    m_mapOfValidCells;
    ListOfMoves        m_validMoves;

    mutable HiResTimer m_timer;

    void blockPosition  (CoordPos const&);
    void blockNeighbours(CoordPos const&);

    void testPosition(BlokusMove&, ShapeCoordPos const&);
    void testShape   (BlokusMove&                      );
    void testPiece   (BlokusMove&, Piece         const&);

    void recalcListOfContactPnts();
	void recalcMapOfValidCells();
    void calcListOfValidMoves();
};