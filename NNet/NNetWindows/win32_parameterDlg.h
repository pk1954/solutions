// win32_parameterDlg.h
//
// NNetWindows

#pragma once

#include "win32_baseDialog.h"

class Param;
class NNetModelCommands;
class NNetModelWriterInterface;

class ParameterDialog : public BaseDialog
{
public:
	ParameterDialog();
	~ParameterDialog() final;

	void Start(HWND const, NNetModelCommands * const, Param * const);
	void Stop();

	void Notify(bool const) final;

private:
	static int const HORZ_SPACE {  8 };
	static int const VERT_SPACE { 14 };

	Param             * m_pParams   { nullptr };
	NNetModelCommands * m_pCommands { nullptr };

	HWND m_hwndBaseFrequency   { nullptr };
	HWND m_hwndPeakVoltage     { nullptr };
	HWND m_hwndThreshold       { nullptr };     
	HWND m_hwndPulseWidth      { nullptr };
	HWND m_hwndRefractoryPeriod{ nullptr };
	HWND m_hwndTimeResolution  { nullptr };
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
