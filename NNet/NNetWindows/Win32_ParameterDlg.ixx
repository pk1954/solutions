// Win32_ParameterDlg.ixx
//
// NNetWindows

module;

#include "NNetModelWriterInterface.h"

export module ParameterDialog;

import BaseDialog;
import NNetModelCommands;
import NNetParameters;

export class ParameterDialog : public BaseDialog
{
public:
	ParameterDialog();
	~ParameterDialog() final;

	void Start(HWND const, NNetModelCommands * const);
	void Stop();
	void SetModelInterface(NNetModelWriterInterface * const);
	void Notify(bool const) final;

private:
	static int const HORZ_SPACE {  8 };
	static int const VERT_SPACE { 14 };

	NNetModelWriterInterface * m_pNMWI     { nullptr };
	NNetModelCommands        * m_pCommands { nullptr };

	HWND m_hwndPeakVoltage     { nullptr };
	HWND m_hwndThreshold       { nullptr };     
	HWND m_hwndSpikeWidth      { nullptr };
	HWND m_hwndRefractoryPeriod{ nullptr };
	HWND m_hwndTimeResolution  { nullptr };
	HWND m_hwndFilterSize      { nullptr };
	HWND m_hwndPulseSpeed      { nullptr };

	void resetParameter   (HWND const, ParamType::Value const) const;
	void applyParameter   (HWND const, ParamType::Value const);
	HWND addParameter     (HWND const, ParamType::Value const, int &);
	void resetParameters();
	void applyParameters();

	ParameterDialog             (ParameterDialog const &);  // noncopyable class 
	ParameterDialog & operator= (ParameterDialog const &);  // noncopyable class 

	bool OnCommand(WPARAM const, LPARAM const, PixelPoint const = PixelPoint::NULL_VAL()) final;
	bool UserProc(UINT const, WPARAM const, LPARAM const) final ;
};
