// win32_NNetController.h
//
// NNetSimu

#pragma once

class NNetModelReaderInterface;
class NNetModelWriterInterface;
class AnimationThread;
class SlowMotionRatio;
class NNetModelStorage;
class DisplayFunctor;
class ComputeThread;
class NNetAppMenu;
class NNetModel;
class MainWindow;
class WinManager;
class Preferences;
class CommandStack;
class Param;
class Sound;

using std::wstring;

class NNetController
{
public:
	NNetController() {}

	void Initialize
	( 
		NNetModelStorage         * const,
		MainWindow               * const,
		WinManager               * const,
		NNetModelReaderInterface * const,
		NNetModelWriterInterface * const,
		ComputeThread            * const,
		SlowMotionRatio          * const,
		DisplayFunctor           * const,
		Sound                    * const,
		Preferences              * const,
		CommandStack             * const
	);

	virtual ~NNetController( );

	bool HandleCommand( int const, LPARAM const, MicroMeterPoint const );

private:

	void pulseRateDlg       ( ShapeId const );
	void triggerSoundDlg    ( ShapeId const );
	bool changePulseRate    ( ShapeId const, bool const );
	bool processUIcommand   ( int const, LPARAM const );
	bool processModelCommand( int const, LPARAM const, MicroMeterPoint const );

	HCURSOR                    m_hCrsrWait             { nullptr };
	Sound                    * m_pSound                { nullptr };
	NNetModelStorage         * m_pStorage              { nullptr };
	MainWindow               * m_pMainWindow           { nullptr };
	WinManager               * m_pWinManager           { nullptr };
	ComputeThread            * m_pComputeThread        { nullptr };
	NNetModelReaderInterface * m_pModelReaderInterface { nullptr };
	NNetModelWriterInterface * m_pModelWriterInterface { nullptr };
	SlowMotionRatio          * m_pSlowMotionRatio      { nullptr };
	DisplayFunctor           * m_pStatusBarDisplay     { nullptr };
	AnimationThread          * m_pAnimationThread      { nullptr };
	Preferences              * m_pPreferences          { nullptr };
	CommandStack             * m_pCommandStack         { nullptr };
};				          
