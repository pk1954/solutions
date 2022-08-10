// UPSensorList.cpp
//
// NNetModel

#include "Sensor.h"
#include "UPSensorList.h"

using std::to_wstring;
using std::make_unique;
using std::ranges::find_if;

UPSensorList::~UPSensorList() {}

UPSensor UPSensorList::removeSensor(vector<UPSensor>::iterator it)
{
    if (it == m_list.end())
        return UPSensor(nullptr);
    else
    {
        UPSensor upSensor = move(*it);
        if (upSensor.get() == GetSensorSelected() )
            SetActive(SensorId(0));
        m_list.erase(it);
        return move(upSensor);
    }
}

vector<UPSensor>::iterator UPSensorList::getSensor(SensorId const id)
{
    return m_list.begin() + id.GetValue();
}

vector<UPSensor>::const_iterator UPSensorList::getSensor(SensorId const id) const
{
    return m_list.begin() + id.GetValue();
}

SensorId UPSensorList::SetActive(SensorId const id)
{
    SensorId sensorIdOld { m_sensorIdSelected };
    m_sensorIdSelected = id;
    return sensorIdOld;
}

void UPSensorList::InsertSensor(UPSensor upSensor, SensorId const id)
{
    m_list.insert(getSensor(id), move(upSensor));
}

Sensor const * UPSensorList::GetSensor(SensorId const id) const
{
    return IsValid(id) ? m_list.at(id.GetValue()).get() : nullptr;
}

Sensor * UPSensorList::GetSensor(SensorId const id)
{
    return IsValid(id) ? m_list.at(id.GetValue()).get() : nullptr;
}

SensorId UPSensorList::FindSensor(MicroMeterPnt const & umPos)
{
    vector<UPSensor>::const_iterator it = 
    {
        find_if(m_list, [&umPos](auto & it){ return it->Includes(umPos); }        )
    };
    return (it == m_list.end()) 
           ? SensorId::NULL_VAL() 
           : SensorId(Cast2Int(it - m_list.begin()));
}

UPSensor UPSensorList::NewSensor
(
    MicroMeterCircle const & circle,
    UPNobList        const & list
)
{
    return move(make_unique<Sensor>(circle, list));
}

SensorId UPSensorList::PushSensor(UPSensor upSensor)
{
    m_list.push_back(move(upSensor));
    return SensorId(Cast2Int(m_list.size()-1));
}

UPSensor UPSensorList::PopSensor()
{
    unique_ptr upSensor { move(m_list.back()) };
    m_list.pop_back();
    return move(upSensor);
}

UPSensor UPSensorList::RemoveSensor(SensorId const id)
{
    return move(removeSensor(getSensor(id)));
}
