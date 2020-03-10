// AutoOpen.h
//
// NNetSimu

#pragma once

class AutoOpen
{
public:

	static void On ( ) 
	{ 
		m_bActive = true; 
	}

	static void Off( ) 
	{ 
		m_bActive = false; 
	}

	static bool IsOn( ) 
	{ 
		return m_bActive; 
	}

private:

	static bool m_bActive;
};
