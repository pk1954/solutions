// Match.ixx
//
// BlokusCore

export module BlokusCore:Match;

import std;
import Types;
import Color;
import HiResTimer;
import :BlokusMove;
import :Board;
import :Components;
import :MatchProtocol;
import :Player;
import :PlayerId;
import :RuleServerInterface;
import :StrategyRandom;
import :StrategyTakeFirst;
import :StrategyHuman;

using std::array;
using std::vector;
using std::unique_ptr;
using std::make_unique;

StrategyHuman     StrategyRed;
StrategyTakeFirst StrategyGreen;
StrategyTakeFirst StrategyBlue;
StrategyRandom    StrategyYellow;

export using PLAYERS = array<Player, NR_OF_PLAYERS>;

export class Match
{
public:
    Match();

    static unique_ptr<Match> CreateNewMatch()
    {
        unique_ptr<Match> upMatch { make_unique<Match>() };
        return upMatch;
    }

    void Reset();
    void ResetTimers();

    void Initialize()
    {
        m_players[0].Initialize(m_board, PlayerId(0), &StrategyRed);
        m_players[1].Initialize(m_board, PlayerId(1), &StrategyGreen); 
        m_players[2].Initialize(m_board, PlayerId(2), &StrategyBlue);
        m_players[3].Initialize(m_board, PlayerId(3), &StrategyYellow); 
    }

    Player       &GetPlayer (PlayerId const id)       { return m_players.at(id.GetValue()); }
    Player const &GetPlayerC(PlayerId const id) const { return m_players.at(id.GetValue()); }
    Player       &ActivePlayer ()                     { return GetPlayer(m_idActivePlayer); }
    Player const &ActivePlayerC()               const { return GetPlayerC(m_idActivePlayer); }

    bool AllShapeCellsUnblocked(BlokusMove const&) const;

    void CalcListOfValidMoves();

    ListOfMoves   m_validMoves;
    PlayerId      m_idActivePlayer { 0 };
    PLAYERS       m_players;
    Board         m_board;
    MatchProtocol m_protocol;
    HiResTimer    m_timerFindContactPnts;
    HiResTimer    m_timer;
                  
private:

    void testPosition(BlokusMove&, ShapeCoordPos const&);
    void testShape   (BlokusMove&                      );
    void testPiece   (BlokusMove&, Piece         const&);
};
