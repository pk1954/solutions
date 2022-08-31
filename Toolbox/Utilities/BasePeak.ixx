// BasePeak.ixx
//
// Toolbox\Utilities

module;

#include <iostream>
#include <iomanip>
#include <algorithm>

export module BasePeak;

using std::min;
using std::max;
using std::setw;
using std::wostream;

export template <typename UNIT>
class BasePeak
{
public:
	BasePeak(UNIT b, UNIT p)
	  : base(b),
		peak(p)
	{}

	bool operator==(BasePeak const& rhs) const = default;

	UNIT Base()  const { return base; }
	UNIT Peak()  const { return peak; }
	UNIT Boost() const { return peak - base; }

	void SetBase(UNIT v)
	{
		base = min(v, peak);
	}

	void SetPeak(UNIT v)
	{
		peak = max(v, base);
	}

	friend wostream & operator<< (wostream& out, BasePeak<UNIT> const& data)
	{
		out << setw(5) << data.base << L' ' << data.peak << L' ';
		return out;
	}
private:
	UNIT base;
	UNIT peak;
};
