// PlayerTypes.ixx
//
// BlokusCore

export module BlokusCore:PlayerTypes;

import std;
import Color;
import :PlayerId;
import :Components;
import :BlokusCoords;

using std::array;
using std::wstring;

export struct PlayerType
{
    CoordPos m_startPoint;
    Color    m_color;
    wstring  m_wstrName;
};

export class PlayerTypes
{
public:
    static void Initialize();

    static PlayerType const& GetPlayerType(PlayerId const);

private:

    static array<PlayerType, NR_OF_PLAYERS> m_playerTypes;
};