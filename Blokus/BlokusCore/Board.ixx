// Board.ixx
//
// BlokusCore

export module BlokusCore:Board;

import std;
import :Components;
import :BlokusUtilities;
import :PieceTypeId;
import :PlayerId;

using std::array;

export class Board
{
public:
    Board();

    void Reset();
    void DoMove  (BlokusMove const&);
    void UndoMove(BlokusMove const&);

    PlayerId    GetPlayerId   (CoordPos const& pos) const { return getCellC(pos).m_idPlayer; }
    PieceTypeId GetPieceTypeId(CoordPos const& pos) const { return getCellC(pos).m_idPieceType; }

    bool IsFreeCell  (CoordPos const&)                 const;
    bool IsContactPnt(CoordPos const&, PlayerId const) const;

    void Apply2AllFreeCellsC(auto const& func) const
    {
    	Apply2AllBoardCells
	    (
		    [this, &func](CoordPos const& coordPos) 
		    {  
			    if (IsFreeCell(coordPos))
                    func(coordPos);
    		}
	    );
    }

    void Apply2AllShapeCells
    (
        BlokusMove const& move,
        auto       const& func
    )
    {
        GetShapeC(move).Apply2AllShapeCellPositionsC
        (
            [this, &move, &func](ShapeCoordPos const &shapePos)
            {
                func(getCell(move, shapePos));
            }
        );
    }

private:
    bool hasPlayerId    (CoordPos const&, PlayerId const) const;
    bool hasDiagContact (CoordPos const&, PlayerId const) const;
    bool hasOrthoContact(CoordPos const&, PlayerId const) const;

    struct Cell
    {
        PlayerId    m_idPlayer;
        PieceTypeId m_idPieceType;
        void reset()
        {
            m_idPlayer    = NO_PLAYER;
            m_idPieceType = UndefinedPieceTypeId;
        }
        void set
        (
            PlayerId    const idPlayer,
            PieceTypeId const idPieceType
        )
        {
            m_idPlayer    = idPlayer;
            m_idPieceType = idPieceType;
        }
};

    array<array<Cell, BOARD_SIZE>, BOARD_SIZE> m_cells;  // affected by move

    Cell       &getCell (CoordPos const pos)       { return m_cells.at(pos.GetYvalue()).at(pos.GetXvalue()); }
    Cell const &getCellC(CoordPos const pos) const { return m_cells.at(pos.GetYvalue()).at(pos.GetXvalue()); }

    Cell &getCell(BlokusMove const& move, ShapeCoordPos const &shapePos)
    {
        CoordPos const coordPos { move.GetCoordPos() + shapePos };
        Assert(IsOnBoard(coordPos));
        return getCell(coordPos);
    }
};