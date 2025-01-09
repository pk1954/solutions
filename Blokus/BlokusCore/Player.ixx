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
import ObserverInterface;
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
using std::ranges::subrange;

export using ListOfContactPnts = vector<CoordPos>;

export class Player : public ObserverInterface
{
public:
    void Initialize(Board const&, PlayerId const, Strategy * const);
    void Dump() const;
    void Reset();
    void ResetTimer() { m_timer.Reset(); }
    bool IsFirstMove() const { return m_bFirstMove; }

    void Notify(bool const) final;

    void               Prepare()                                             const;
    ListOfMoves const &GetListOfValidMoves()                                 const;
    BlokusMove         SelectMove(RuleServerInterface const&)                const;
    bool               AnyShapeCellsBlocked(BlokusMove const)                const;
    bool               AnyShapeCellsBlocked(Shape const&, CoordPos const &)  const;
    void               DrawFreePieces (DrawContext&, Piece const * const)    const;
    void               DrawSetPieces  (DrawContext&)                         const;
    void               DrawContactPnts(DrawContext&)                         const;
    void               DrawResult     (DrawContext&, TextFormatHandle const) const;

    bool            IsMoveable(PieceTypeId const  id ) const { return m_mapOfMoveablePieces.at(id.GetValue()); }
    bool            IsBlocked (CoordPos    const &pos) const { return m_mapOfValidCells.IsBlocked(pos); }
    bool            IsHuman()                          const { return GetStrategy().IsHuman(); }
    bool            HasFinished()                      const { return m_listOfValidMoves.Empty(); }
    int             Result()                           const { return m_iResult; }
    Ticks           GetTicks()                         const { return m_timer.GetAccumulatedActionTicks(); }
    Color           GetColor()                         const { return m_pPlayerType->m_color; }
    wstring  const &GetName()                          const { return m_pPlayerType->m_wstrName; }
    Strategy const &GetStrategy()                      const { return *m_pStrategy; }
    Piece    const &GetPieceC (PieceTypeId const  id ) const { return m_pieces.at(id.GetValue()); }
    Piece          &GetPiece  (PieceTypeId const  id )       { return m_pieces.at(id.GetValue()); }
    PlayerId        GetPlayerId()                      const { return m_idPlayer; }

    subrange<MoveIter> GetMoves(PieceTypeId const id)  const { return m_listOfValidMoves.GetMoves(id); }

    void DoMove(BlokusMove);

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
        for (CoordPos const& pos : m_listOfContactPnts)
            func(pos);
    }

    bool IsTrue4AnyContactPnt(auto const& func) const
    {
        for (CoordPos const& pos : m_listOfContactPnts)
            if (func(pos))
                return true;
        return false;
    }

private:
    bool               m_bFirstMove;       
    Board      const * m_pBoard;
    PlayerType const * m_pPlayerType;
    Strategy         * m_pStrategy;
    PieceTypeId        m_idPieceTypeLastMove;
    PlayerId           m_idPlayer;

    array<Piece, NR_OF_PIECE_TYPES> m_pieces;

    // tables with lazy evaluation
    mutable bool                           m_bTablesValid { false };
    mutable MapOfValidCells                m_mapOfValidCells;
    mutable ListOfContactPnts              m_listOfContactPnts;
    mutable ListOfMoves                    m_listOfValidMoves;
    mutable array<bool, NR_OF_PIECE_TYPES> m_mapOfMoveablePieces;

    mutable HiResTimer m_timer;
    mutable int        m_iResult;          

    void testPosition(BlokusMove&, ShapeCoordPos const&) const;
    void testShape   (BlokusMove&)                       const;
    void testPiece   (BlokusMove&, Piece         const&) const;

    int  calcResult() const;

    void recalcListOfContactPnts() const;
 	void recalcMapOfValidCells  () const;
 	void calcListOfValidMoves   () const;

    void dumpContactPnts() const;
    void dumpListOfValidMoves() const;
    void dumpMapOfMoveablePieces() const;
};