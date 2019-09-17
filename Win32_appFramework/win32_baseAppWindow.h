// win32_baseAppWindow.h
//
// Win32_appFramework

#pragma once

class ModelWindow;
class HistWindow;
class StatusBar;

class BaseAppWindow
{
public:
	static void AdjustChildWindows( ModelWindow *, HistWindow *, StatusBar * );
};