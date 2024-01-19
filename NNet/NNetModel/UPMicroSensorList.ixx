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
import :UPNobList;
import :MicroSensor;

using std::ranges::for_each;
using std::ranges::find_if;
using std::make_unique;
using std::unique_ptr;
using std::wstring;
using std::vector;
using std::move;

export class UPMicroSensorList
{
public:
    ~UPMicroSensorList() {}

	void Clear() { m_list.clear(); }

    size_t Size() const { return m_list.size(); }

	void AddMicroSensor(unique_ptr<MicroSensor> upMicroSensor)
	{
		m_list.push_back(move(upMicroSensor));
	}

	MicroSensor * GetMicroSensor(NobId const id)
	{
		vector<unique_ptr<MicroSensor>>::iterator itMS { getMicroSensor(id) };
		if (itMS == m_list.end())
			return nullptr;
		else
			return itMS->get();
	}

	MicroSensor const * GetMicroSensorC(NobId const id) const
	{
		vector<unique_ptr<MicroSensor>>::const_iterator itMS { getMicroSensorC(id) };
		if (itMS == m_list.end())
			return nullptr;
		else
			return itMS->get();
	}

	unique_ptr<MicroSensor> RemoveMicroSensor(NobId const id)
	{
		vector<unique_ptr<MicroSensor>>::iterator it { getMicroSensor(id) };
		if (it == m_list.end())
			return nullptr;
		unique_ptr<MicroSensor> upMicroSensor { move(*it) };
		m_list.erase(it);
		return move(upMicroSensor);
	}

	bool HasMicroSensor(NobId const id) const
	{
		return getMicroSensorC(id) != m_list.end();
	}

private:

    vector<unique_ptr<MicroSensor>> m_list;

	vector<unique_ptr<MicroSensor>>::iterator getMicroSensor(NobId const id)
	{
		return find_if(m_list, [id](auto const& it) { return it->GetNobId() == id; });
	}

	vector<unique_ptr<MicroSensor>>::const_iterator getMicroSensorC(NobId const id) const
	{
		return find_if(m_list, [id](auto const& it) { return it->GetNobId() == id; });
	}

	unique_ptr<MicroSensor> removeMicroSensor(vector<unique_ptr<MicroSensor>>::iterator it)
	{
		if (it == m_list.end())
			return unique_ptr<MicroSensor>(nullptr);
		else
			return move(*it);
	}

};
