// Win32_BaseDialog.ixx
//
// Win32_utilities

module;

#include <memory>
#include <Windows.h>

export module BaseDialog;

import Direct2D;
import RootWindow;

using std::unique_ptr;

static INT_PTR CALLBACK BaseDialogProc(HWND const, UINT const, WPARAM const, LPARAM const);

export class BaseDialog: public RootWindow
{
public:

	HWND StartBaseDialog(HWND const, VisCrit const &);

	D2D_driver * StartGraphics();
	D2D_driver * GetGraphics() { return m_upGraphics.get(); }

	virtual bool UserProc(UINT const, WPARAM const, LPARAM const);
	
	void Stop() override;

protected:

	virtual void PaintGraphics() {};

	unique_ptr<D2D_driver> m_upGraphics { nullptr };

private:

	friend static INT_PTR CALLBACK BaseDialogProc(HWND const, UINT const, WPARAM const, LPARAM const);

	bool OnSize(PIXEL const, PIXEL const);
};
