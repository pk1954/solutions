// win32_NNetController.h
//
// NNetSimu

#pragma once

#include "ArrowAnimation.h"

class NNetModelReaderInterface;
class NNetModelCommands;
class SlowMotionRatio;
class MonitorWindow;
class NNetModelExporter;
class DisplayFunctor;
class ComputeThread;
class NNetAppMenu;
class NNetModel;
class MainWindow;
class WinManager;
class Preferences;
class CommandStack;
class Signal;
class Sound;

using std::wstring;

class NNetController
{
public:
	NNetController() {}

	void Initialize
	( 
		NNetModelExporter        * const,
		MainWindow               * const,
		WinManager               * const,
		NNetModelReaderInterface * const,
		NNetModelCommands        * const,
		ComputeThread            * const,
		SlowMotionRatio          * const,
		DisplayFunctor           * const,
		Sound                    * const,
		Preferences              * const,
		CommandStack             * const,
		MonitorWindow            * const
	);

	virtual ~NNetController( );

	bool HandleCommand( int const, LPARAM const, MicroMeterPoint const = NP_NULL );

private:

	void pulseRateDlg       ( ShapeId const );
	void triggerSoundDlg    ( ShapeId const );
	bool processUIcommand   ( int const, LPARAM const );
	bool processModelCommand( int const, LPARAM const, MicroMeterPoint const );

	ArrowAnimation m_arrowAnimation;

	HCURSOR                    m_hCrsrWait         { nullptr };
	Sound                    * m_pSound            { nullptr };
	NNetModelExporter        * m_pModelExporter    { nullptr };
	MainWindow               * m_pMainWindow       { nullptr };
	WinManager               * m_pWinManager       { nullptr };
	ComputeThread            * m_pComputeThread    { nullptr };
	NNetModelReaderInterface * m_pNMRI              { nullptr };
	NNetModelCommands        * m_pModelCommands    { nullptr };
	SlowMotionRatio          * m_pSlowMotionRatio  { nullptr };
	DisplayFunctor           * m_pStatusBarDisplay { nullptr };
	Preferences              * m_pPreferences      { nullptr };
	CommandStack             * m_pCommandStack     { nullptr };
	MonitorWindow            * m_pMonitorWindow    { nullptr };
};				          
