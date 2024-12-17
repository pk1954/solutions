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
    void NextPlayer()  { m_pMatch->NextPlayer(); }
    void PrevPlayer()  { m_pMatch->PrevPlayer(); }

    void ResetPiece(BlokusMove const move) { getPiece(move).Reset(); }
    void DoMove    (BlokusMove);
    void UndoMove  (BlokusMove);
    void Finalize() { m_pMatch->ActivePlayer().Finalize(); }

private:
    Player &getPlayer (PlayerId const id) { return m_pMatch->GetPlayer(id); }
    Player &activePlayer()                { return m_pMatch->ActivePlayer(); }
    Piece  &getPiece(BlokusMove const move)
    {
	    Player &player { getPlayer(move.GetPlayerId()) };
	    return player.GetPiece(move.GetPieceTypeId());
    }
};
