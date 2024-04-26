// NNetViewerAppWindow.ixx
//
// NNetViewer

module;

#include <string>
#include <fstream>
#include <iostream>
#include <Windows.h>

export module NNetViewerAppWindow;

import AboutBox;
import BaseWindow;
import MessagePump;
import NNetModel;
import NNetModelIO;
import NNetViewerAppMenu;
import StatusBar;
import StatusBarDisplayFunctor;

using std::wstring;
using std::wofstream;
using std::unique_ptr;

export class NNetViewerAppWindow : public BaseWindow
{
public:
	NNetViewerAppWindow(wstring const &);
	~NNetViewerAppWindow() override;

	virtual void Start(MessagePump&);
	virtual void Stop();

	void DoGameStuff() { }

	NNetViewerAppWindow(NNetViewerAppWindow const&) = delete;  // noncopyable class 
	NNetViewerAppWindow& operator= (NNetViewerAppWindow const&) = delete;  // noncopyable class 

private:

	bool OnCommand(WPARAM const, LPARAM const, PixelPoint const) final;
	void OnNotify (WPARAM const, LPARAM const)                   final;
	void OnChar   (WPARAM const, LPARAM const)                   final;
	bool OnSize   (PIXEL  const, PIXEL  const)                   final;
	void OnClose  ()                                             final;

	void    configureStatusBar();
	wstring AskModelFile(enum class tFileMode const) const;

	bool m_bStarted { false }; // if true, model is visible, all functions available

	int                        m_statusMessagePart{ };
	unique_ptr<Model>          m_upModel;
	NNetModelReaderInterface * m_pNMRI       { nullptr };
	HWND                       m_hwndApp     { nullptr };
	AboutBox                   m_aboutBox;
	NNetModelIO                m_modelIO;
	NNetViewerAppMenu          m_appMenu;
	StatusBar                  m_statusBar;
	StatusBarDisplayFunctor    m_statusBarDispFunctor;

	bool UserProc(UINT const, WPARAM const, LPARAM const) override;
};
