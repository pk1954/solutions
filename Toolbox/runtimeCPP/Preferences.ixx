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

import BoolType;
import SoundInterface;

using std::wostream;
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
	
	virtual void WriteAppPreferences(wostream&) const {};

	void AddWrapper(unique_ptr<WrapBase> upWrapper)
	{
		m_prefVector.push_back(move(upWrapper));
	}

	void AddBoolWrapper(wstring const& name, BoolType& boolType)
	{
		AddWrapper(make_unique<WrapBaseBool>(name, boolType));
	}

	inline static BoolType m_bColorMenu { false };
	inline static BoolType m_bAutoOpen  { true };
	inline static BoolType m_bSound     { false };

private:

	vector<unique_ptr<WrapBase>> m_prefVector;
	wstring                      m_wstrPreferencesFile;
};
