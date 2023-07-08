// AutoOpen.ixx
//
// Toolbox\Utilities

export module AutoOpen;

export class AutoOpen
{
public:

	static void On()
	{
		m_bActive = true;
	}

	static void Off()
	{
		m_bActive = false;
	}

	static bool IsOn()
	{
		return m_bActive;
	}

private:

	inline static bool m_bActive{ true };
};
