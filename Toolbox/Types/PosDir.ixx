// PosDir.ixx
//
// Toolbox\Types

export module Types:PosDir;

import :BasicTypes;
import :MoreTypes;

export class PosDir
{
public:
    PosDir()
      : m_umPos(NP_NULL),
        m_degrees(0._Degrees)
    {}

    PosDir(MicroMeterPnt const &pos, Degrees degrees)
      : m_umPos(pos),
        m_degrees(degrees)
    {}

	bool operator==(PosDir const& rhs) const = default;

    PosDir operator+= (PosDir const a) { m_umPos += a.m_umPos; m_degrees += a.m_degrees; return * this; }
	PosDir operator-= (PosDir const a) { m_umPos -= a.m_umPos; m_degrees -= a.m_degrees; return * this; }

	PosDir operator*= (float const d) { m_umPos *= d; m_degrees *= d; return * this; }
	PosDir operator/= (float const d) { m_umPos /= d; m_degrees /= d; return * this; }

	friend PosDir operator+ (PosDir const a, PosDir const b) 
	{ 
		PosDir res { a }; 
		res += b; 
		return res; 
	};

	friend PosDir operator- (PosDir const a, PosDir const b) 
	{ 
		PosDir res { a }; 
		res -= b; 
		return res; 
	};

	friend PosDir operator* (PosDir const a,  float const d) 
	{ 
		PosDir res { a }; 
		res *= d; 
		return res; 
	};

    MicroMeterPnt m_umPos;
    Degrees       m_degrees;
};