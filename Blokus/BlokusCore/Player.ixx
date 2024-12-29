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

export using ListOfContactPnts = vector<CoordPos>;

export class Player : public ObserverInterface
{
public:
    void Initialize(Board const&, PlayerId const, Strategy * const);
    void Reset();
    void ResetTimer() { m_timer.Reset(); }
    bool IsFirstMove() const { return m_bFirstMove; }

    void Notify(bool const) final;

    wstring const &GetName() const { return m_pPlayerType->m_wstrName; }

    Piece const &GetPieceC (PieceTypeId const id) const { return m_pieces.at(id.GetValue()); }
    Piece       &GetPiece  (PieceTypeId const id)       { return m_pieces.at(id.GetValue()); }
    bool         IsMoveable(PieceTypeId const id) const { return m_mapOfMoveablePieces.at(id.GetValue()); }

    void DrawFreePieces (DrawContext&, Piece const * const)    const;
    void DrawSetPieces  (DrawContext&)                         const;
    void DrawContactPnts(DrawContext&)                         const;
    void DrawResult     (DrawContext&, TextFormatHandle const) const;

    void Prepare() const;
    ListOfMoves const &GetListOfValidMoves() const;
    BlokusMove SelectMove(RuleServerInterface const&) const;

    void DoMove  (BlokusMove&);
    void UndoMove();
    void Finalize();
    void UndoFinalize();

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

    bool AnyShapeCellsBlocked(BlokusMove const&) const;

    bool IsBlocked(CoordPos const &pos) const { return m_mapOfValidCells.IsBlocked(pos); }

    bool            HasFinished() const { return m_bFinished; }
    int             Result()      const { return m_iResult; }
    Ticks           GetTicks()    const { return m_timer.GetAccumulatedActionTicks(); }
    Strategy const &GetStrategy() const { return *m_pStrategy; }
    Color           GetColor()    const { return m_pPlayerType->m_color; }
    bool            IsHuman()     const { return GetStrategy().IsHuman(); }

private:
    using PIECE_TYPE_SET = array<Piece, NR_OF_PIECE_TYPES>;

    // directly affected by move
    unsigned int m_remainingPieces;  
    int          m_iResult;          
    bool         m_bFinished;        
    bool         m_bFirstMove;       

    // not directly affected by move
    Board      const * m_pBoard;
    PlayerType const * m_pPlayerType;
    Strategy         * m_pStrategy;  //TODO: move from here to MatchReaderInterface?
    PlayerId           m_idPlayer;
    PIECE_TYPE_SET     m_pieces;

    // tables with lazy evaluation
    mutable bool              m_bTablesValid { false };
    mutable MapOfValidCells   m_mapOfValidCells;
    mutable ListOfContactPnts m_listOfContactPnts;
    mutable ListOfMoves       m_listOfValidMoves;
    mutable array<bool, NR_OF_PIECE_TYPES> m_mapOfMoveablePieces;

    mutable HiResTimer m_timer;

    void testPosition(BlokusMove&, ShapeCoordPos const&) const;
    void testShape   (BlokusMove&)                       const;
    void testPiece   (BlokusMove&, Piece         const&) const;

    void recalcListOfContactPnts() const;
 	void recalcMapOfValidCells  () const;
 	void calcListOfValidMoves   () const;
};