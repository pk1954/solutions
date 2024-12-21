// NextMoveCmd.ixx
//
// BlokusCommands

export module BlokusCommands:NextMoveCmd;

import std;
//import AnimationCommand;
import :BlokusCommand;

//using PosDirAnimationCmd = AnimationCommand<PosDir>;

using std::wstring;

export class NextMoveCmd : public BlokusCommand  //PosDirAnimationCmd
{
public:

    NextMoveCmd(BlokusMove const move)
      : m_move(move)
    {}

	void Do() final 
	{
		if (m_move.IsDefined())
       	    m_pMWI->DoMove(m_move);
        else 
            m_pMWI->Finalize();
        m_pMWI->NextPlayer();
}

	void Undo() final 
	{
		if (m_move.IsDefined())
       	    m_pMWI->UndoMove(m_move);
        else 
            m_pMWI->UndoFinalize();
	    m_pMWI->PrevPlayer();
    }

    static void Register()
    {
        SymbolTable::ScrDefConst(NAME, &m_wrapper);
    }

    static void Push(BlokusMove const move)
    {
        //if (IsTraceOn())
        //    TraceStream() << NAME << umAnimated << SPACE << bOn << endl;

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
            //MicroMeter animated { ScrReadMicroMeter(script) };
            //bool       bOn      { script.ScrReadBool() };
            //ArrowAnimationCmd::Push(animated, bOn, false);
        }
    } m_wrapper { NAME };
};