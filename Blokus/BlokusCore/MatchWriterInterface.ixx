// MatchWriterInterface.ixx
//
// BlokusCore

export module BlokusCore:MatchWriterInterface;

import :BlokusMove;
import :MatchReaderInterface;
import :Player;
import :PlayerId;

using std::array;
using std::vector;
using std::unique_ptr;

export class MatchWriterInterface : public MatchReaderInterface
{
public:
    void SetMatch(Match *pMatch) { m_pMatch = pMatch; }

    void Reset()       { m_pMatch->Reset(); }
    void ResetTimers() { m_pMatch->ResetTimers(); }

    void       ResetPiece(BlokusMove const move) { getPiece(move).Reset(); }
    BlokusMove DoMove    (BlokusMove);
    void       UndoMove  (BlokusMove const);
    void       Finalize() { m_pMatch->ActivePlayer().Finalize(); }
    PlayerId   NextPlayer();

private:
    Player &getPlayer (PlayerId const id) { return m_pMatch->GetPlayer(id); }
    Player &activePlayer()                { return m_pMatch->ActivePlayer(); }
    Piece  &getPiece(BlokusMove const move)
    {
	    Player &player { getPlayer(move.GetPlayerId()) };
	    return player.GetPiece(move.GetPieceTypeId());
    }
};
