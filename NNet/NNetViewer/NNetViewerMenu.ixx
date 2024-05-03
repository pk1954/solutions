// NNetViewerMenu.ixx
//
// NNetViewer

module;

#include <string>
#include <memory>
#include <Windows.h>

export module NNetViewerMenu;

import ObserverInterface;

using std::wstring;
using std::unique_ptr;

export class NNetViewerMenu : public ObserverInterface
{
public:
	NNetViewerMenu();
	~NNetViewerMenu() final;

	void Start(HWND const);
	void Notify(bool const) final;

private:

	HWND  m_hwndScanViewerWindow { nullptr };
	HMENU m_hMenu                { nullptr };
};