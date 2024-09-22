// Game.cpp
//
// BlokusCore

module BlokusCore:Game;

int iAvailablePieces { 0 };
int iNrOfShapes      { 0 };
int iNrOfContactPnts { 0 };

void Game::FindValidMoves(PlayerId const idPlayer)
{
    m_validMoves.clear();
    Move move;
    move.m_idPlayer = idPlayer;
    Player const& player { m_players.GetPlayerC(idPlayer) };
    if (!player.IsFirstMove())
        FindContactPnts(m_activePlayer);
    player.Apply2FreePiecesC
    (
        [this, &move](Piece const& piece)
        {
            move.m_idPieceType = piece.GetPieceTypeId();
            ++iAvailablePieces;
		    Components::GetPieceTypeC(move.m_idPieceType).Apply2AllShapeIdsC
		    (
			    [this, &move](ShapeId const& idShape)
			    {
                    move.m_idShape = idShape;
                    ++iNrOfShapes;
				    m_board.GetShapeC(move, m_players).Apply2AllCornerPntsC
				    (
					    [this, &move](ShapeCoordPos const &posCorner) 
                        { 
                            m_players.GetPlayerC(move.m_idPlayer).Apply2AllContactPntsC
                            (
                                [this, &move, &posCorner](CoordPos const& posContact)
                                {
                                    move.m_boardPos = posContact - posCorner;
                                    ++iNrOfContactPnts;
                                    if (m_board.IsValidMove(move, m_players))
                                    {
                                        m_validMoves.push_back(move);
                                        if (
                                              (move.m_idPlayer.GetValue() == 1) &&
                                              (move.m_idPieceType.GetValue() == 10)
                                           )
                                            m_board.IsValidMove(move, m_players);
                                    }
                                }
				            );
			            }
		            );
                }
            );
        }
    );
}
