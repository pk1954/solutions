// NobType.ixx
//
// NNetModel

module;

#include <cassert>
#include <iostream>
#include <functional>

export module NNetModel:NobType;

import Debug;

using std::wostream;
using std::wstring;
using std::size_t;

export class NobType
{
public:
	enum class Value
	{
		inputConnector,
		outputConnector,
		inputLine,
		outputLine,
		neuron,
		knot,
		pipe,
		undefined,
		nobTypeLast = pipe,
		nobTypeFirst = inputConnector
	};

	static size_t const NR_OF_NOB_TYPES { static_cast<size_t>(Value::nobTypeLast) + 1 };

	NobType()
		: m_value(Value::undefined)
	{}

	NobType(Value val)   // "explicit" makes trouble!
		: m_value(val)
	{}

	bool operator==(NobType const& rhs) const = default;

	bool operator!=(NobType const& rhs) const
	{
		return !(m_value == rhs.m_value);
	}

	void Check() const
	{
		AssertLimits<int>((int)m_value, (int)Value::nobTypeFirst, (int)Value::undefined);
	}

	static constexpr void Apply2All(auto const& func)
	{
		for (int i = 0; i <= static_cast<int>(NobType::Value::nobTypeLast); ++i)
			func(static_cast<Value>(i));
	}

	static constexpr wstring GetListOfNobTypes()
	{
		wstring wstrList;
		NobType::Apply2All
		(
			[&wstrList](NobType::Value const& t)
			{
				wstrList += NobType::GetName(t) + L" ";
			}
		);
		return wstrList;
	}

	static wstring        GetName(NobType::Value const);
	static NobType::Value GetTypeFromName(wstring const&);

	bool IsPipeType           () const { return m_value == Value::pipe; }
	bool IsDefinedType        () const { return m_value != Value::undefined; }
	bool IsUndefinedType      () const { return m_value == Value::undefined; }
	bool IsKnotType           () const { return m_value == Value::knot; }
	bool IsNeuronType         () const { return m_value == Value::neuron; }
	bool IsInputLineType      () const { return m_value == Value::inputLine; }
	bool IsOutputLineType     () const { return m_value == Value::outputLine; }
	bool IsInputConnectorType () const { return m_value == Value::inputConnector; }
	bool IsOutputConnectorType() const { return m_value == Value::outputConnector; }

	bool IsIoConnectorType() const
	{
		return IsInputConnectorType() || IsOutputConnectorType();
	}

	bool IsIoLineType() const
	{
		return (m_value == Value::inputLine) || (m_value == Value::outputLine);
	}

	bool IsAnyNeuronType() const
	{
		return (m_value == Value::neuron) || IsIoLineType();
	}

	bool IsBaseKnotType() const
	{
		return IsAnyNeuronType() || (m_value == Value::knot);
	}

	Value GetValue() const { return m_value; }

	friend wostream& operator<< (wostream&, NobType const& nobType);

private:
	Value m_value;
};
