// NNetViewerAppMenu.ixx
//
// NNetWindows

module;

#include <string>
#include <memory>
#include <Windows.h>

export module NNetViewerAppMenu;

import ObserverInterface;

using std::wstring;
using std::unique_ptr;

export class NNetViewerAppMenu : public ObserverInterface
{
public:
	NNetViewerAppMenu();
	~NNetViewerAppMenu() final;

	void Start(HWND const);
	void Notify(bool const) final;

private:

	HWND  m_hwndApp { nullptr };
	HMENU m_hMenu   { nullptr };
};