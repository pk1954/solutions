// UPSensorList.cpp
//
// NNetModel

module NNetModel:UPSensorList;

import std;
import SaveCast;
import Types;
import :Sensor;
import :MicroSensor;
import :UPNobList;

using std::vector;
using std::to_wstring;
using std::unique_ptr;
using std::make_unique;
using std::ranges::find_if;

void UPSensorList::Clear()
{
    m_list.clear();
}

vector<UPSensor>::iterator UPSensorList::getSensor(SensorId const id)
{
    return m_list.begin() + id.GetValue();
}

vector<UPSensor>::const_iterator UPSensorList::getSensor(SensorId const id) const
{
    return m_list.begin() + id.GetValue();
}

void UPSensorList::AddSensor(UPSensor upSensor, SensorId const id)
{
    m_list.insert(getSensor(id), move(upSensor));
}

SensorId UPSensorList::AddSensor(UPSensor upSensor)
{
    m_list.push_back(move(upSensor));
    return SensorId(Cast2Int(m_list.size()-1));
}

Sensor const * UPSensorList::GetSensor(SensorId const id) const
{
    return IsValid(id) ? m_list.at(id.GetValue()).get() : nullptr;
}

Sensor * UPSensorList::GetSensor(SensorId const id)
{
    return IsValid(id) ? m_list.at(id.GetValue()).get() : nullptr;
}

SensorId UPSensorList::FindSensor(MicroMeterPnt const & umPos) const
{
    vector<UPSensor>::const_iterator it 
    { 
        find_if
        (
            m_list, 
            [&umPos](UPSensor const & upSensor) 
            { 
                return (upSensor) ? upSensor->Includes(umPos) : false;
            }
        ) 
    };
    return (it == m_list.end())
        ? SensorId::NULL_VAL()
        : SensorId(Cast2Int(it - m_list.begin()));
}

SensorId UPSensorList::GetSensorId(Sensor const& sensor) const
{
    vector<UPSensor>::const_iterator it { find_if(m_list, [&sensor](auto const& it) { return it.get() == &sensor; }) };
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
    return make_unique<Sensor>(circle, list);
}

UPSensor UPSensorList::RemoveSensor(SensorId const id)
{
    vector<UPSensor>::iterator it { getSensor(id) };
    if (it == m_list.end())
        return UPSensor(nullptr);
    else
        return move(*it);
}
