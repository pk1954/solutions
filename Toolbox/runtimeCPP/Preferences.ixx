// Preferences.ixx
//
// Toolbox\runtimeCPP

module;

#include <compare>
#include <string>
#include <vector>
#include <memory>

export module Preferences;

import WrapBase;

using std::wstring;
using std::vector;
using std::unique_ptr;
using std::make_unique;

export class Preferences
{
public:
	void Initialize(wstring const&);

	bool ReadPreferences() const;
	bool WritePreferences() const;

	bool ColorMenuVisible() const { return m_bColorMenu; }
	void SetColorMenu(bool const b) { m_bColorMenu = b; }

	void AddWrapper(unique_ptr<WrapBase> upWrapper)
	{
		m_prefVector.push_back(move(upWrapper));
	}

	//template <Wrap_t WRAPPER>
	//void Add(wstring const& name)
	//{
	//	m_prefVector.push_back(make_unique<WRAPPER>(name, *this));
	//}

private:
	vector<unique_ptr<WrapBase>> m_prefVector;
	wstring                      m_wstrPreferencesFile;
	bool                         m_bColorMenu { false };
};
