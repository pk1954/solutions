// GridPOI.ixx
//

export module EvolutionCore:GridPOI;

import :EvolutionTypes;
import :GridPoint;

class GridPOI  // point of interest
{
public:
	static void SetPoi(GridPoint const);

	static void ClearPoi()
	{
		m_gpPOI.Set2Null();
	}

	static bool IsPoiDefined()
	{
		return m_gpPOI.IsNotNull();
	}

	static bool IsPoi(GridPoint const gp)
	{
		return gp == m_gpPOI;
	}

	static GridPoint const GetPoi()
	{
		return m_gpPOI;
	}

private:
	static GridPoint m_gpPOI;
};
