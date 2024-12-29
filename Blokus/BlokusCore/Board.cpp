// Board.cpp
//
// BlokusCore

module BlokusCore:Board;
    
Board::Board()
{
    Reset();
}

void Board::Reset()
{
    for (int i = 0; i < BOARD_SIZE; ++i) 
    for (int j = 0; j < BOARD_SIZE; ++j)
        m_cells[i][j].reset();
    NotifyAll(false);
 }

bool Board::IsFreeCell(CoordPos const& pos) const
{
    return IsOnBoard(pos) && (GetPlayerId(pos) == NO_PLAYER);
}

bool Board::IsContactPnt(CoordPos const& pos, PlayerId const id) const
{
	if (!IsFreeCell(pos))
        return false;
    if (!hasDiagContact(pos, id))
        return false;
    if (hasOrthoContact(pos, id))
        return false;
    return true;
}

void Board::DoMove(BlokusMove const& move)   // Set affected shape cells to player id
{
    Apply2AllShapeCells
    (
        move, 
        [&move](Cell &cell){ cell.set(move.GetPlayerId(), move.GetPieceTypeId()); }
    );
    NotifyAll(false);
}

void Board::UndoMove(BlokusMove const& move)   // Set affected shape cells to NO_PLAYER
{
    Apply2AllShapeCells
    (
        move,
        [](Cell &cell){ cell.reset(); }
    );
    NotifyAll(false);
}

// A cell in the environment of a cell is a contact point 
// if at least one diagonal neighbour of the cell belongs to the same player
// and no orthogonal (horz or vert) neighbour of the cell belongs to the same player

bool Board::hasPlayerId(CoordPos const& pos, PlayerId const id) const
{
    return IsOnBoard(pos) && (GetPlayerId(pos) == id);
}

bool Board::hasDiagContact(CoordPos const& pos, PlayerId const id) const
{
	return (hasPlayerId(NorthEastPos(pos), id)) || 
           (hasPlayerId(SouthEastPos(pos), id)) ||
		   (hasPlayerId(NorthWestPos(pos), id)) || 
           (hasPlayerId(SouthWestPos(pos), id));
}

bool Board::hasOrthoContact(CoordPos const& pos, PlayerId const id) const
{
    return (hasPlayerId(NorthPos(pos), id)) ||
           (hasPlayerId(EastPos (pos), id)) ||
		   (hasPlayerId(SouthPos(pos), id)) ||
           (hasPlayerId(WestPos (pos), id));
}
