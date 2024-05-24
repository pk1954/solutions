// ScanViewer.ixx
//
// NNetViewer

module;

#include <memory>
#include <Windows.h>

export module ScanViewer;

import Types;
import Tooltip;
import NNetPreferences;
import NNetModel;
import NNetWin32;

using std::unique_ptr;

export class ScanViewer : public NNetWindow
{
public:
	ScanViewer(HWND const, NNetModelReaderInterface const * const, mV const&);

	RawImage const& GetImage   () const;
	float           AspectRatio() const;

private:
	void PaintGraphics() final;
	bool OnSize(PIXEL const, PIXEL const);
	void centerAndZoomRect();

	class Controller: public NNetCommandHandler
	{
	public:
		bool HandleCommand(int const, LPARAM const, MicroMeterPnt const = NP_NULL) final;
	private:
	};				          

	Controller           m_controller;
	unique_ptr<RawImage> m_upFiltered;
	mV            const& m_mVmaxPixel;
	UP_TTIP              m_upToolTip;
};