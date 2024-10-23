// NextMoveCmd.ixx
//
// BlokusCommands

export module BlokusCommands:NextMoveCmd;

import Types;
import AnimationCommand;
import BlokusCore;
import Resource;

using PosDirAnimationCmd = AnimationCommand<PosDir>;

export class NextMoveCmd : public PosDirAnimationCmd
{
public:

    NextMoveCmd
    (
        RootWindow   & rootWin,
        PosDir       & posDirAnimated,
        PosDir const & posDirTarget
    )
        : PosDirAnimationCmd(rootWin, posDirAnimated, posDirAnimated, posDirTarget)
    {}

    void UpdateUI() final
    {
        PosDirAnimationCmd::UpdateUI();
        NotifyAnimWin(true);
		if (TargetReached())
			PostCommand2AnimWin(IDX_FINISH_MOVE);
    };

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
        //Player          &player    { m_match.GetPlayer(m_move.GetPlayerId()) };
        //Piece           &piece     { player.GetPiece  (m_move.GetPieceTypeId()) };
        //PieceType const &pieceType { piece.GetPieceTypeC() };
        //Shape     const &shape     { pieceType.GetShapeC(m_move.GetShapeId()) };
        //PosDir          &posDirAct { piece.GetPosDir() };
        //m_posDirTarget = PosDir(Convert2fCoord(m_move.GetCoordPos()), 0._Degrees);
        //if (posDirTarget == posDirAnimated)
        //    return;

        //if (IsTraceOn())
        //    TraceStream() << NAME << umAnimated << SPACE << bOn << endl;

        //if (bAnimation)
        //    PushCommand
        //    (
        //        make_unique<ArrowAnimationCmd>
        //        (
        //            *WinManager::GetRootWindow(RootWinId(IDM_MAIN_WINDOW)),
        //            umAnimated, 
        //            umAnimated, 
        //            umTarget
        //        )
        //    );
        //else
        //    umAnimated = umTarget;
    //static void Initialize()
    //{
	   // m_posDirAnimation.SetUpdateLambda
	   // (
		  //  [this](bool const bTargetReached)
		  //  {
			 //   PostCommand(IDX_ANIMATION_UPDATE);
			 //   if (bTargetReached)
				//    PostCommand(IDX_FINISH_MOVE);
		  //  }
	   // );
    //}

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