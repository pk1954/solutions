// NextMoveCmd.ixx
//
// BlokusCommands

export module BlokusCommands:NextMoveCmd;

import std;
import Types;
import Commands;
import WinCommand;
import WinManager;
import Win32_Util_Resource;
//import AnimationCommand;
import BlokusCore;
import Resource;

//using PosDirAnimationCmd = AnimationCommand<PosDir>;

using std::wstring;
using std::make_unique;

export class NextMoveCmd : public WinCommand  //PosDirAnimationCmd
{
public:

    NextMoveCmd
    (
        Match           &match,
        BlokusMove const move
    )
      : m_match(match),
        m_move(move)
    {}

    void UpdateUI() final
    {
        WinManager::GetRootWindow(RootWinId(IDM_MAIN_WINDOW))->Notify(false);
    };

	void Do() final 
	{
   	    m_move = m_match.DoMove(m_move);
    }

	void Undo() final 
	{
   	    m_match.UndoMove(m_move);
	}

    static void Register()
    {
        SymbolTable::ScrDefConst(NAME, &m_wrapper);
    }

    static void Push
    (
        Match           &match,
        BlokusMove const move
    )
    {
        //if (IsTraceOn())
        //    TraceStream() << NAME << umAnimated << SPACE << bOn << endl;

        PushCommand(make_unique<NextMoveCmd>(match, move));
    }

private:
    inline static const wstring NAME { L"NextMoveCmd" };

    BlokusMove m_move;
    Match    & m_match;

    inline static struct myWrapper : public Wrapper
    {
        using Wrapper::Wrapper;
        void operator() (Script& script) const final
        {
            //MicroMeter animated { ScrReadMicroMeter(script) };
            //bool       bOn      { script.ScrReadBool() };
            //ArrowAnimationCmd::Push(animated, bOn, false);
        }
    } m_wrapper { NAME };
};