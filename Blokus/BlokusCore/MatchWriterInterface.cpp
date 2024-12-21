// MatchWriterInterface.cpp
//
// BlokusCore

module BlokusCore:MatchWriterInterface;

import Debug;

void MatchWriterInterface::DoMove(BlokusMove move)
{
    Assert(move.IsDefined());
    Assert(!activePlayer().HasFinished());
	getPiece(move).DoMove(move);     // Set piece pos and mark as set
    m_pMatch->m_board.DoMove(move);  // Set affected cells to player id
    activePlayer().DoMove(move);     // may finish, if all pieces set
}

void MatchWriterInterface::UndoMove(BlokusMove move)
{
    Assert(move.IsDefined());
    getPiece(move).Reset();             // Reset piece pos and mark as available
    m_pMatch->m_board.UndoMove(move);   // Set affected cells to NO_PLAYER
    m_pMatch->GetPlayer(move.GetPlayerId()).UndoMove();
}
