// NextMoveCmd.ixx
//
// BlokusCommands

export module BlokusCommands:NextMoveCmd;

import std;
import Debug;
import SaveCast;
import RunTime;
import :BlokusCommand;

using std::wstring;

export class NextMoveCmd : public BlokusCommand
{
public:

    NextMoveCmd(BlokusMove const move)
      : m_move(move)
    {}

	void Do() final 
	{
        Assert(m_move.IsDefined());
        m_pMWI->DoMove(m_move);
    }

    static void Register()
    {
        SymbolTable::ScrDefConst(NAME, &m_wrapper);
    }

    static void Push(BlokusMove const move)
    {
		if (IsTraceOn())
			TraceStream() << NAME << SPACE << move << endl;
        PushCommand(make_unique<NextMoveCmd>(move));
    }

private:
    inline static const wstring NAME { L"NextMoveCmd" };

    BlokusMove m_move;

    inline static struct myWrapper : public Wrapper
    {
        using Wrapper::Wrapper;
        void operator() (Script& script) const final
        {
            PlayerId    const idPlayer    { Cast2SignedChar(script.ScrReadInt()) };
            PieceTypeId const idPieceType { script.ScrReadUchar() };
            ShapeId     const idShape     { Cast2SignedChar(script.ScrReadChar()) };
            CoordPos    const coordPos    { ScrReadCoordPos(script) };
            BlokusMove  const move(idPlayer, idPieceType, idShape, coordPos);
            NextMoveCmd::Push(move);
        }
    } m_wrapper { NAME };
};