// NNetViewerWindow.ixx
//
// NNetViewer

module;

#include <string>
#include <fstream>
#include <iostream>
#include <Windows.h>
#include <memory>
#include <vector>

export module NNetViewerWindow;

import BaseWindow;
import NNetModel;
import NNetModelIO;
import NNetViewerMenu;
import StatusBar;
import StatusBarDisplayFunctor;
import ScanPanel;

using std::wstring;
using std::wofstream;
using std::unique_ptr;
using std::vector;

export class NNetViewerWindow : public BaseWindow
{
public:
	NNetViewerWindow(wstring const &);
	~NNetViewerWindow() override;

	void Start();

    void AddScan(unique_ptr<ScanPanel> panel)
    {
		m_panelList.push_back(move(panel));
		arrangePanels(GetClRectSize());
    }

	NNetViewerWindow            (NNetViewerWindow const&) = delete;  // noncopyable class 
	NNetViewerWindow& operator= (NNetViewerWindow const&) = delete;  // noncopyable class 

private:

	bool OnCommand(WPARAM const, LPARAM const, PixelPoint const) final;
	void OnNotify (WPARAM const, LPARAM const)                   final;
	void OnChar   (WPARAM const, LPARAM const)                   final;
	bool OnSize   (PIXEL  const, PIXEL  const)                   final;
	void OnClose  ()                                             final;

	void    arrangePanels(PixelRectSize const&);
	void    configureStatusBar();
	wstring AskModelFile(enum class tFileMode const) const;

	bool m_bStarted { false }; // if true, model is visible, all functions available

	int                           m_statusMessagePart{ };
	HWND                          m_hwndApp     { nullptr };
	NNetModelIO                   m_modelIO;
	NNetViewerMenu                m_scanViewerMenu;
	StatusBar                     m_statusBar;
	StatusBarDisplayFunctor       m_statusBarDispFunctor;
    vector<unique_ptr<ScanPanel>> m_panelList;

	bool UserProc(UINT const, WPARAM const, LPARAM const) override;
};
