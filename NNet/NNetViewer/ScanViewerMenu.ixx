// ScanViewerMenu.ixx
//
// NNetWindows

module;

#include <string>
#include <memory>
#include <Windows.h>

export module ScanViewerMenu;

import ObserverInterface;

using std::wstring;
using std::unique_ptr;

export class ScanViewerMenu : public ObserverInterface
{
public:
	ScanViewerMenu();
	~ScanViewerMenu() final;

	void Start(HWND const);
	void Notify(bool const) final;

private:

	HWND  m_hwndScanViewerWindow { nullptr };
	HMENU m_hMenu                { nullptr };
};