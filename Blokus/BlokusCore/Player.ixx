// Player.ixx
//
// BlokusCore

export module BlokusCore:Player;

import std;
import Types;
import Color;
import Direct2D;
import :BlokusDrawContext;
import :BoardMap;
import :Piece;
import :PlayerId;
import :PieceTypeId;
import :Components;
import :BlokusCoords;
import :Move;

using std::array;
using std::vector;
using std::wstring;

export class Player
{
public:
    void Initialize(BlokusCoordPos const, Color const, wstring const&);

    bool IsFirstMove() const { return m_bFirstMove; }

    wstring const& GetName() const { return m_wstrColor; }

    Piece const& GetPieceC(PieceTypeId const id)  { return m_pieces.at(id.GetValue()); }
    Piece      & GetPiece (PieceTypeId const id)  { return m_pieces.at(id.GetValue()); }

    void DrawFreePieces (BlokusDrawContext&) const;
    void DrawContactPnts(BlokusDrawContext&) const;
    void DrawResult     (BlokusDrawContext&, TextFormatHandle const) const;
    void DrawCell       (BlokusDrawContext&, BlokusCoordPos const&) const;

    Move const& SelectMove(vector<Move> const&) const;
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

    void Apply2FreePiecesC(auto const& func) const
    {
        for (Piece const& piece: m_pieces)
            if (piece.IsAvailable())
                func(piece);
    }

    void Apply2AllContactPntsC(auto const& func) const
    {
        for (BlokusCoordPos const& pos : m_contactPntsOnBoard)
            func(pos);
    }

    void ClearContactPnts()
    {
        m_contactPntsOnBoard.clear();
    }

    void AddContactPnt(BlokusCoordPos const &pos)
    {
        m_contactPntsOnBoard.push_back(pos);
    }

    bool IsValidPos(BlokusCoordPos const &pos) const 
    { 
        return m_validPositions.IsValidPos(pos); 
    }

    void DoFinish();

    bool HasFinished() const { return m_bFinished; }
    int  Result()      const { return m_iResult; }

private:
   // static Random m_random;

    PieceTypeId                     m_pieceTypeIdMove; 
    unsigned int                    m_remainingPieces; 
    int                             m_iResult;
    bool                            m_bFinished;       
    bool                            m_bFirstMove;      
    Color                           m_color;
    array<Piece, NR_OF_PIECE_TYPES> m_pieces;
    vector<BlokusCoordPos>          m_contactPntsOnBoard;
    BoardMap                        m_validPositions;
    wstring                         m_wstrColor;

    void reduceValidMoves(Move const&, PieceType const&);
};