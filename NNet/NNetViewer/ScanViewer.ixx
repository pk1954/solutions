// ScanViewer.ixx
//
// NNetViewer

export module ScanViewer;

import Types;
import NNetWin32;

export class ScanViewer : public NNetWindow
{
public:
private:
	void PaintGraphics() final
	{

	}

	void centerAndZoomRect()
	{
		CenterAndZoomRect(GetCoord(), m_pNMRI->GetScanAreaRect(), 1.0f);
	}
};