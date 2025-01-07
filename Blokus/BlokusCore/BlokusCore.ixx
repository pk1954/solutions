// BlokusCore.ixx
//
// BlokusCore

export module BlokusCore;

export import :BlokusMove;
export import :BlokusPreferences;
export import :BlokusUtilities;
export import :Components;
export import :ListOfMoves;
export import :MatchReaderInterface;
export import :MatchWriterInterface;
export import :PieceMotion;
export import :Shape;
//export import :Strategy;
//export import :StrategyTakeFirst;
//export import :StrategyRandom;
export import :Tournament;

export void InitializeBlokusCore()
{
	Components::Initialize();
	Shape::Initialize();
}