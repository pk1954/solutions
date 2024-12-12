// MatchWriterInterface.cpp
//
// BlokusCore

module BlokusCore:MatchWriterInterface;

import Debug;

BlokusMove MatchWriterInterface::DoMove(BlokusMove move)
{
    Assert(move.IsDefined());
    Assert(!activePlayer().HasFinished());
	getPiece(move).DoMove(move.GetCoordPos());
    m_pMatch->m_board.DoMove(move);
    activePlayer().DoMove(move);     // may finish, if all pieces set
    move.SetPlayerId(NextPlayer());
    return move;
}

void MatchWriterInterface::UndoMove(BlokusMove const move)
{
    Assert(move.IsDefined());
    getPiece(move).Reset();  // Undo move for piece
    m_pMatch->m_board.UndoMove(move);
    m_pMatch->GetPlayer(move.GetPlayerId()).UndoMove();
    m_pMatch->m_idActivePlayer = move.GetPlayerId();
}

PlayerId MatchWriterInterface::NextPlayer()
{
    if (++m_pMatch->m_idActivePlayer > LAST_PLAYER)
        m_pMatch->m_idActivePlayer = FIRST_PLAYER;    
    m_pMatch->ActivePlayer().RecalcListOfContactPnts();
    m_pMatch->CalcListOfValidMoves();
    return m_pMatch->m_idActivePlayer;
}
