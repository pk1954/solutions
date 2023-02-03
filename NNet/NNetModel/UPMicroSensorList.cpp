// UPMicroSensorList.cpp
//
// NNetModel

module;

#include <string>
#include <memory>
#include <vector>
#include <algorithm>

module NNetModel:UPMicroSensorList;

import SaveCast;
import Types;
import :MicroSensor;
import :UPNobList;

using std::vector;
using std::to_wstring;
using std::unique_ptr;
using std::make_unique;
using std::ranges::find_if;

UPMicroSensorList::~UPMicroSensorList() {}

UPMicroSensor UPMicroSensorList::removeMicroSensor(vector<UPMicroSensor>::iterator it)
{
    if (it == m_list.end())
        return UPMicroSensor(nullptr);
    else
    {
        UPMicroSensor upMicroSensor = move(*it);
        if (upMicroSensor.get() == GetMicroSensorSelected())
            SetActive(MicroSensorId(0));
        return move(upMicroSensor);
    }
}

vector<UPMicroSensor>::iterator UPMicroSensorList::getMicroSensor(MicroSensorId const id)
{
    return m_list.begin() + id.GetValue();
}

vector<UPMicroSensor>::const_iterator UPMicroSensorList::getMicroSensor(MicroSensorId const id) const
{
    return m_list.begin() + id.GetValue();
}

MicroSensorId UPMicroSensorList::SetActive(MicroSensorId const id)
{
    MicroSensorId microSensorIdOld { m_microSensorIdSelected };
    m_microSensorIdSelected = id;
    return microSensorIdOld;
}

void UPMicroSensorList::InsertMicroSensor(UPMicroSensor upMicroSensor, MicroSensorId const id)
{
    m_list.insert(getMicroSensor(id), move(upMicroSensor));
}

MicroSensor const* UPMicroSensorList::GetMicroSensor(MicroSensorId const id) const
{
    return IsValid(id) ? m_list.at(id.GetValue()).get() : nullptr;
}

MicroSensor * UPMicroSensorList::GetMicroSensor(MicroSensorId const id)
{
    return IsValid(id) ? m_list.at(id.GetValue()).get() : nullptr;
}

MicroSensorId UPMicroSensorList::FindMicroSensor(NobId const nobId) const
{
    vector<UPMicroSensor>::const_iterator it { find_if(m_list, [&nobId](auto& it) { return it ? (it->GetNobId() == nobId) : false; }) };
    return (it == m_list.end())
        ? MicroSensorId::NULL_VAL()
        : MicroSensorId(Cast2Int(it - m_list.begin()));
}

MicroSensorId UPMicroSensorList::GetMicroSensorId(MicroSensor const& microSensor) const
{
    vector<UPMicroSensor>::const_iterator it { find_if(m_list, [&microSensor](auto const& it) { return it.get() == &microSensor; }) };
    return (it == m_list.end())
        ? MicroSensorId::NULL_VAL()
        : MicroSensorId(Cast2Int(it - m_list.begin()));
}

UPMicroSensor UPMicroSensorList::NewMicroSensor(Nob * const pNob)
{
    return move(make_unique<MicroSensor>(pNob));
}

MicroSensorId UPMicroSensorList::PushMicroSensor(UPMicroSensor upMicroSensor)
{
    m_list.push_back(move(upMicroSensor));
    return MicroSensorId(Cast2Int(m_list.size() - 1));
}

UPMicroSensor UPMicroSensorList::PopMicroSensor()
{
    unique_ptr<MicroSensor> upMicroSensor { move(m_list.back()) };
    m_list.pop_back();
    return move(upMicroSensor);
}

UPMicroSensor UPMicroSensorList::RemoveMicroSensor(MicroSensorId const id)
{
    return move(removeMicroSensor(getMicroSensor(id)));
}
