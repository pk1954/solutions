// UPSensorList.ixx
//
// NNetModel

module;

#include <string>
#include <memory>
#include <vector>
#include <algorithm>

export module NNetModel:UPSensorList;

import Types;
import :UPNobList;
import :Sensor;

using std::ranges::for_each;
using std::unique_ptr;
using std::wstring;
using std::vector;

export using SensorId = NamedType<int, struct SensorIdParam>;
export using UPSensor = unique_ptr<Sensor>;

export class UPSensorList
{
public:
    ~UPSensorList() {};

    void Clear();

    size_t Size() const { return m_list.size(); }

    Sensor const * GetSensor(SensorId const) const;
    Sensor       * GetSensor(SensorId const);
    SensorId       GetSensorId(Sensor const&) const;

    bool IsValid(SensorId const id) const { return id.GetValue() < m_list.size(); }

    SensorId PushSensor(UPSensor);
    UPSensor PopSensor();
    UPSensor RemoveSensor(SensorId const);
    void     InsertSensor(UPSensor, SensorId const);

    void Apply2All(auto const & f)
    {
        for_each(m_list, [&f](auto & up) { f(up.get()); });
    }

    void Apply2AllC(auto const & f) const
    {
        for_each(m_list, [&f](auto const & up) { f(up.get()); });
    }

    UPSensor NewSensor(MicroMeterCircle const &, UPNobList const &);
    SensorId FindSensor(MicroMeterPnt const&) const;
    SensorId FindSensor(NobId const) const;

private:

    vector<UPSensor> m_list;

    vector<UPSensor>::iterator       getSensor(SensorId const);
    vector<UPSensor>::const_iterator getSensor(SensorId const) const;

    UPSensor removeSensor(vector<UPSensor>::iterator);
};
