// NNetViewerWindow.ixx
//
// NNetViewer

module;

#include <string>
#include <fstream>
#include <iostream>
#include <Windows.h>
#include <memory>

export module NNetViewerWindow;

import BaseWindow;
import NNetModel;
import NNetModelIO;
import ScriptHook;
import StatusBar;
import PanelPlatform;
import StatusBarDisplayFunctor;

using std::wstring;
using std::wofstream;
using std::unique_ptr;

export class NNetViewerWindow : public BaseWindow
{
public:
	NNetViewerWindow();
	~NNetViewerWindow() override;

	NNetViewerWindow            (NNetViewerWindow const&) = delete;  // noncopyable class 
	NNetViewerWindow& operator= (NNetViewerWindow const&) = delete;  // noncopyable class 

private:

	bool OnCommand(WPARAM const, LPARAM const, PixelPoint const) final;
	bool OnSize   (PIXEL  const, PIXEL  const)                   final;
	//void OnClose  ()                                             final;

	void configureStatusBar();

	int                       m_statusMessagePart;
	NNetModelIO               m_modelIO;
	StatusBar                 m_statusBar;
	ScriptHook                m_ScriptHook;
	unique_ptr<PanelPlatform> m_upPanelPlatform;
	StatusBarDisplayFunctor   m_statusBarDispFunctor;
};
