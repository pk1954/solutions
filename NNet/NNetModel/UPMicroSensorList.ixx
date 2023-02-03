// UPMicroSensorList.ixx
//
// NNetModel

module;

#include <string>
#include <memory>
#include <vector>
#include <algorithm>

export module NNetModel:UPMicroSensorList;

import Types;
import :NobId;
import :UPNobList;
import :MicroSensor;

using std::ranges::for_each;
using std::unique_ptr;
using std::wstring;
using std::vector;

export using MicroSensorId = NamedType<int, struct MicroSensorIdParam>;
export using UPMicroSensor = unique_ptr<MicroSensor>;

export class UPMicroSensorList
{
public:
    ~UPMicroSensorList();

    size_t Size() const { return m_list.size(); }

    MicroSensor const * GetMicroSensor(MicroSensorId const) const;
    MicroSensor       * GetMicroSensor(MicroSensorId const);
    MicroSensorId       GetMicroSensorId(MicroSensor const&) const;

    MicroSensorId       GetMicroSensorIdSelected() const { return m_microSensorIdSelected; }
    MicroSensor const * GetMicroSensorSelected  () const { return GetMicroSensor(m_microSensorIdSelected); }
    MicroSensor       * GetMicroSensorSelected  ()       { return GetMicroSensor(m_microSensorIdSelected); }

    bool IsAnyMicroSensorSelected()      const { return m_microSensorIdSelected.IsNotNull(); }
    bool IsValid(MicroSensorId const id) const { return id.GetValue() < m_list.size(); }

    MicroSensorId SetActive(MicroSensorId const);
    MicroSensorId PushMicroSensor(UPMicroSensor);
    UPMicroSensor PopMicroSensor();
    UPMicroSensor RemoveMicroSensor(MicroSensorId const);
    void          InsertMicroSensor(UPMicroSensor, MicroSensorId const);

    void Apply2All(auto const& f)
    {
        for_each(m_list, [&f](auto& up) { f(up.get()); });
    }

    void Apply2AllC(auto const& f) const
    {
        for_each(m_list, [&f](auto const& up) { f(up.get()); });
    }

    UPMicroSensor NewMicroSensor(Nob * const);
    MicroSensorId FindMicroSensor(NobId const) const;

private:

    vector<UPMicroSensor> m_list;
    MicroSensorId         m_microSensorIdSelected { MicroSensorId::NULL_VAL() };

    vector<UPMicroSensor>::iterator       getMicroSensor(MicroSensorId const);
    vector<UPMicroSensor>::const_iterator getMicroSensor(MicroSensorId const) const;

    UPMicroSensor removeMicroSensor(vector<UPMicroSensor>::iterator);
};
