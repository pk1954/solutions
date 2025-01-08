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

    void ResetPiece(BlokusMove const move) { m_pMatch->GetPiece(move).Reset(); }
    void DoMove    (BlokusMove const move) { return m_pMatch->DoMove(move); }

    Player &GetPlayer(PlayerId   const id)   { return m_pMatch->GetPlayer(id); }
    Player &GetPlayer(BlokusMove const move) { return m_pMatch->GetPlayer(move); }
};
