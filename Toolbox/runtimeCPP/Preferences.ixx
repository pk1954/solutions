// Preferences.ixx
//
// Toolbox\runtimeCPP

module;

#include <compare>
#include <string>
#include <iostream>
#include <vector>
#include <memory>

export module Preferences;

export import WrapBase;

import SoundInterface;

using std::wostream;
using std::wstring;
using std::vector;
using std::unique_ptr;
using std::make_unique;

export class Preferences
{
public:
	void Initialize
	(
		wstring const&,
		Sound*
	);

	bool ReadPreferences() const;
	bool WritePreferences() const;
	
	virtual void WriteAppPreferences(wostream&) const {};

	static bool ColorMenuVisible() { return m_bColorMenu; }
	static void SetColorMenu(bool const b) { m_bColorMenu = b; }

	Sound& GetSound() { return *m_pSound; }

	void AddWrapper(unique_ptr<WrapBase> upWrapper)
	{
		m_prefVector.push_back(move(upWrapper));
	}

private:

	template <Wrap_t WRAPPER>
	void Add(wstring const& name)
	{
		AddWrapper(make_unique<WRAPPER>(name, *this));
	}

	vector<unique_ptr<WrapBase>> m_prefVector;
	wstring                      m_wstrPreferencesFile;
	Sound                      * m_pSound     { nullptr };

	inline static bool m_bColorMenu { false };
};
