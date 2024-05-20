// ScanViewer.ixx
//
// NNetViewer

module;

#include <Windows.h>

export module ScanViewer;

import Types;
import NNetPreferences;
import NNetWin32;

export class ScanViewer : public NNetWindow
{
public:
	ScanViewer(HWND const, NNetModelReaderInterface const * const);

	float AspectRatio() const;

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

	Controller m_controller;
};