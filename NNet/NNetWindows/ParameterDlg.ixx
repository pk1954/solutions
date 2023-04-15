// ParameterDlg.ixx
//
// NNetWindows

module;

#include <compare>
#include <memory>
#include <Windows.h>

export module NNetWin32:ParameterDialog;

import Direct2D;
import BaseDialog;
import NNetModelCommands;
import NNetModel;

using std::unique_ptr;

export class ParameterDialog : public BaseDialog
{
public:
	ParameterDialog();
	~ParameterDialog() final;

	void Start(HWND const, NNetModelCommands* const);
	void Stop();
	void SetModelInterface(NNetModelWriterInterface* const);
	void Notify(bool const) final;
	void PaintGraphics() final;

private:
	static int const HORZ_SPACE { 8 };
	static int const VERT_SPACE { 10 };

	//inline static D2D1::ColorF const D2D_COL_BACKGROUND { D2D1::ColorF::Red };
	inline static COLORREF const COL_BACKGROUND { D2D1::ColorF::Red };

	ID2D1SolidColorBrush* m_brushBackGround;

	NNetModelWriterInterface * m_pNMWI     { nullptr };
	NNetModelCommands        * m_pCommands { nullptr };

	HWND m_hwndPulseFreqMax     { nullptr };
	HWND m_hwndPeakVoltage      { nullptr };
	HWND m_hwndNeuronThreshold  { nullptr };
	HWND m_hwndSynapseDelay     { nullptr };
	HWND m_hwndPulseWidth       { nullptr };
	HWND m_hwndTimeResolution   { nullptr };
	HWND m_hwndFilterSize       { nullptr };
	HWND m_hwndPulseSpeed       { nullptr };

	void resetParameter(HWND const, ParamType::Value const) const;
	void applyParameter(HWND const, ParamType::Value const);
	HWND addParameter  (HWND const, ParamType::Value const, int &);
	void refreshParameters();
	void applyParameters();

	ParameterDialog             (ParameterDialog const &);  // noncopyable class 
	ParameterDialog & operator= (ParameterDialog const &);  // noncopyable class 

	bool OnCommand(WPARAM const, LPARAM const, PixelPoint const = PixelPoint::NULL_VAL()) final;
	bool UserProc(UINT const, WPARAM const, LPARAM const) final;
};
