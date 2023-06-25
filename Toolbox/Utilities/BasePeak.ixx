// BasePeak.ixx
//
// Toolbox\Utilities

module;

#include <algorithm>

export module BasePeak;

using std::min;
using std::max;

export template <std::totally_ordered UNIT>
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
private:
	UNIT base;
	UNIT peak;
};
