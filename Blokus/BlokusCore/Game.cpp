// Game.cpp
//
// BlokusCore

module BlokusCore:Game;

import PerfCounter;
import :RuleServerInterface;

using std::wcout;
using std::endl;
using std::make_unique;
using std::vector;

vector<Move> const& RuleServer::GetListOfValidMoves() const
{
    return m_game.FindValidMoves();
}

Board const & RuleServer::GetBoard() const
{
    return m_game.GetBoard();
}

void Game::Initialize()
{
    m_board.Initialize();
    m_players.Initialize();
    m_protocol.Initialize();
    m_bGameFinished = false;
    m_uiPlayersLeft = NR_OF_PLAYERS;
    m_activePlayer  = FIRST_PLAYER;
    m_timer.Reset();

    m_upRuleServer = make_unique<RuleServer>(*this);
}

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

PlayerId Game::WinnerId()
{
    PlayerId idBestPlayer { NO_PLAYER };
    int      iBestResult { (std::numeric_limits<int>::min)() };
    Apply2AllPlayerIds
    (
        [this, &idBestPlayer, &iBestResult](PlayerId const idPlayer)
        {
            Player const &player { m_players.GetPlayerC(idPlayer) };
            if (player.Result() > iBestResult)
            {
                iBestResult = player.Result();
                idBestPlayer = idPlayer;
            }
        }
    );
    return idBestPlayer;
}

void Game::playerFinished(Player& player)
{
    player.DoFinish();
    if (--m_uiPlayersLeft == 0)
    {
        m_bGameFinished = true;
    	m_timer.AfterAction();
		Ticks const ticks { m_timer.GetSingleActionTicks() };
		wcout << L"Complete game:"<< PerfCounter::Ticks2wstring(ticks) << endl;
    }
}

bool Game::NextPlayer()
{
    if (!m_bGameStarted)
    {
        m_bGameStarted = true;
     	m_timer.BeforeAction();
   }
	if (!ActivePlayer().HasFinished())
	{
		NextMove();
		//Ticks const ticks { m_timer.GetSingleActionTicks() };
		//wcout << L"complete move:"<< PerfCounter::Ticks2wstring(ticks) << endl;
	}
    if (GameFinished())
        return false;

    if (++m_activePlayer > PlayerId(NR_OF_PLAYERS))
        m_activePlayer = PlayerId(FIRST_PLAYER);
	FindContactPnts();
    
    return true;
}

bool Game::NextMove()
{
    Player &player { ActivePlayer() };
    Move move { player.SelectMove(*m_upRuleServer.get()) };
    if (move.Undefined())
    {
        playerFinished(player);
        return false;
    }
    m_protocol.Add(move);
    player.PerformMove(move);
    m_board.PerformMove(move);
    if (player.HasFinished())  
    {
		Ticks const ticks { m_timer.GetAccumulatedActionTicks() };
		wcout << L"all moves:"<< PerfCounter::Ticks2wstring(ticks) << endl;
        playerFinished(player);
        return false;
    }
    return true;
}

void Game::FindContactPnts()
{
    //m_timerFindContactPnts.BeforeAction();
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
    //m_timerFindContactPnts.AfterAction();
    //Ticks const ticks { m_timerFindContactPnts.GetSingleActionTicks() };
    //wcout << L"FindContactPnts:"<< PerfCounter::Ticks2wstring(ticks) << endl;
}

bool Game::isValidMove
(
    Move   const& move,
    Player const& player
)
{
    PieceType const& pieceType { Components::GetPieceTypeC(move.GetPieceTypeId()) };
    Shape     const& shape     { pieceType  .GetShapeC    (move.GetShapeId()) };
    return shape.Apply2AllShapeCellsB
    (
        [this, &move, &player](ShapeCoordPos const &shapePos)
        {
            CoordPos const coordPos { move.GetCoordPos() + shapePos };
            return m_board.IsFreeCell(coordPos) && player.IsValidPos(coordPos);
        }
    );
}

vector<Move> const & Game::FindValidMoves()
{
    g_iNrOfPieces = 0;
    g_iNrOfShapes = 0;
    g_iNrOfMoves  = 0;
    m_timerFindValidMoves.BeforeAction();
    m_validMoves.clear();
    Move move;
    move.SetPlayerId(m_activePlayer);
    ActivePlayer().Apply2FreePiecesC
    (
        [this, &move](Piece const& piece)
        {
            move.SetPieceTypeId(piece.GetPieceTypeId());
            ++g_iNrOfPieces;
		    Components::GetPieceTypeC(move.GetPieceTypeId()).Apply2AllShapeIdsC
		    (
			    [this, &move](ShapeId const& idShape)
			    {
                    move.SetShapeId(idShape);
                    ++g_iNrOfShapes;
				    GetShapeC(move).Apply2AllCornerPntsC
				    (
					    [this, &move](ShapeCoordPos const &posCorner) 
                        { 
                            Player const& player { m_players.GetPlayerC(move.GetPlayerId()) };
                            player.Apply2AllContactPntsC
                            (
                                [this, &player, &move, &posCorner](CoordPos const& posContact)
                                {
                                    move.SetCoordPos(posContact - posCorner);
                                    ++g_iNrOfMoves;
                                    if (isValidMove(move, player))
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

    //m_timerFindValidMoves.AfterAction();
    //Ticks const ticks        { m_timerFindValidMoves.GetSingleActionTicks() };
    //Ticks const ticksPerMove {ticks / g_iNrOfMoves };
    //
    //wcout << L"FindValidMoves:"                     << endl;
    //wcout << g_iNrOfPieces       << L" pieces"      << endl;
    //wcout << g_iNrOfShapes       << L" shapes"      << endl;
    //wcout << g_iNrOfMoves        << L" moves"       << endl;
    //wcout << m_validMoves.size() << L" valid moves" << endl;
    //wcout << PerfCounter::Ticks2wstring(ticks) 
    //      << L"(" << PerfCounter::Ticks2wstring(ticksPerMove) << L" per move)" 
    //      << endl;

    return m_validMoves;
}
