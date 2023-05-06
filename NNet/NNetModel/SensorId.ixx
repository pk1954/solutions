// SensorId.ixx
//
// NNetModel

export module NNetModel:SensorId;

import Types;

export using SensorId = NamedType<int, struct SensorIdParam>;

export inline SensorId const NO_SENSOR(-1);

export inline bool IsDefined(SensorId const id)
{
	return id != NO_SENSOR;
}

export inline bool IsUndefined(SensorId const id)
{
	return id == NO_SENSOR;
}
