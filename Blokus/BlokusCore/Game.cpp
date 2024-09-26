// Game.cpp
//
// BlokusCore

module BlokusCore:Game;

import PerfCounter;

using std::wcout;
using std::endl;

void Game::DrawSetPieces(DrawContext &context) const
{
    Apply2AllBoardCells
    (
        [this, &context](CoordPos const& pos)
        {
		    PlayerId const idPlayer { m_board.GetPlayerId(pos) };
		    if (idPlayer != NO_PLAYER)
		    {
                Player const& player { m_players.GetPlayerC(idPlayer) };
                player.DrawCell(context, pos);
		    }
        }
    );
}

void Game::NextPlayer()
{
    if (++m_activePlayer == PlayerId(NR_OF_PLAYERS))
        m_activePlayer = PlayerId(0);
}

Player const& Game::Winner()
{
    Player const* pBestPlayer { nullptr };
    int           iBestResult { (std::numeric_limits<int>::min)() };
    m_players.Apply2AllPlayersC
    (
        [&pBestPlayer, &iBestResult](Player const &player)
        {
            if (player.Result() > iBestResult)
            {
                iBestResult = player.Result();
                pBestPlayer = &player;
            }
        }
    );
    return *pBestPlayer;
}

void Game::playerFinished(Player& player)
{
    player.DoFinish();
    if (--m_uiPlayersLeft == 0)
        m_bGameFinished = true;
}

bool Game::NextMove()
{
    Player &player { ActivePlayer() };
    FindValidMoves(m_activePlayer);
    if (m_validMoves.empty())
    {
        playerFinished(player);
        return false;
    }
    Move const &move { ActivePlayerC().SelectMove(m_validMoves) };
    player.PerformMove(move);
    m_board.PerformMove(move, m_players);
    if (player.HasFinished())
    {
        playerFinished(player);
        return false;
    }
    return true;
}

void Game::FindContactPnts()
{
    m_timerFindContactPnts.BeforeAction();
    Player &player { m_players.GetPlayer(m_activePlayer) };
    if (!player.IsFirstMove())
    {
        player.ClearContactPnts();
        Apply2AllBoardCells
        (
            [this, &player](CoordPos const& pos)
            {
                if (m_board.IsContactPnt(pos, m_activePlayer))
                {
                    player.AddContactPnt(pos);
                    m_board.IsContactPnt(pos, m_activePlayer);
                }
            }
        );
    }
    m_timerFindContactPnts.AfterAction();
    Ticks const ticks { m_timerFindContactPnts.GetSingleActionTicks() };
    wcout << L"FindContactPnts:"<< PerfCounter::Ticks2wstring(ticks) << endl;
}

void Game::FindValidMoves(PlayerId const idPlayer)
{
    g_iNrOfPieces = 0;
    g_iNrOfShapes = 0;
    g_iNrOfMoves  = 0;
    m_timerFindValidMoves.BeforeAction();
    m_validMoves.clear();
    Move move;
    move.m_idPlayer = idPlayer;
    m_players.GetPlayerC(idPlayer).Apply2FreePiecesC
    (
        [this, &move](Piece const& piece)
        {
            move.m_idPieceType = piece.GetPieceTypeId();
            ++g_iNrOfPieces;
		    Components::GetPieceTypeC(move.m_idPieceType).Apply2AllShapeIdsC
		    (
			    [this, &move](ShapeId const& idShape)
			    {
                    move.m_idShape = idShape;
                    ++g_iNrOfShapes;
				    m_board.GetShapeC(move, m_players).Apply2AllCornerPntsC
				    (
					    [this, &move](ShapeCoordPos const &posCorner) 
                        { 
                            m_players.GetPlayerC(move.m_idPlayer).Apply2AllContactPntsC
                            (
                                [this, &move, &posCorner](CoordPos const& posContact)
                                {
                                    move.m_boardPos = posContact - posCorner;
                                    ++g_iNrOfMoves;
                                    if (m_board.IsValidMove(move, m_players))
                                    {
                                        m_validMoves.push_back(move);
                                    }
                                }
				            );
			            }
		            );
                }
            );
        }
    );
    m_timerFindValidMoves.AfterAction();
    Ticks const ticks        { m_timerFindValidMoves.GetSingleActionTicks() };
    Ticks const ticksPerMove {ticks / g_iNrOfMoves };
    
    wcout << L"FindValidMoves:"                     << endl;
    wcout << g_iNrOfPieces       << L" pieces"      << endl;
    wcout << g_iNrOfShapes       << L" shapes"      << endl;
    wcout << g_iNrOfMoves        << L" moves"       << endl;
    wcout << m_validMoves.size() << L" valid moves" << endl;
    wcout << PerfCounter::Ticks2wstring(ticks) 
          << L"(" << PerfCounter::Ticks2wstring(ticksPerMove) << L" per move)" 
          << endl;
}
