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
import NNetViewerMenu;
import StatusBar;
import PanelPlatform;
import StatusBarDisplayFunctor;

using std::wstring;
using std::wofstream;

export class NNetViewerWindow : public BaseWindow
{
public:
	NNetViewerWindow();
	~NNetViewerWindow() override;

	NNetViewerWindow            (NNetViewerWindow const&) = delete;  // noncopyable class 
	NNetViewerWindow& operator= (NNetViewerWindow const&) = delete;  // noncopyable class 

private:

	bool OnCommand(WPARAM const, LPARAM const, PixelPoint const) final;
	void OnNotify (WPARAM const, LPARAM const)                   final;
	void OnChar   (WPARAM const, LPARAM const)                   final;
	bool OnSize   (PIXEL  const, PIXEL  const)                   final;
	void OnClose  ()                                             final;

	void    configureStatusBar();
	wstring AskModelFile(enum class tFileMode const) const;

	int                     m_statusMessagePart;
	NNetModelIO             m_modelIO;
	NNetViewerMenu          m_scanViewerMenu;
	StatusBar               m_statusBar;
	PanelPlatform           m_panelPlatform;
	StatusBarDisplayFunctor m_statusBarDispFunctor;

	bool UserProc(UINT const, WPARAM const, LPARAM const) override;
};
