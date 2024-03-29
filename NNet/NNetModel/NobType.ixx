// NobType.ixx
//
// NNetModel

module;

#include <cassert>
#include <iostream>
#include <functional>

export module NNetModel:NobType;

import Debug;
import IoUtil;
import IoConstants;

using std::wostream;
using std::wstring;
using std::size_t;
using std::uint8_t;

export class NobType
{
public:
	enum class Value : uint8_t
	{
		inputConnector,
		outputConnector,
		inputLine,
		outputLine,
		neuron,
		knot,
		fork,
		synapse,
		pipe,
		undefined,
		nobTypeLast = undefined,
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
				wstrList += NobType::GetName(t) + SPACE;
			}
		);
		return wstrList;
	}

	static wstring        GetName(NobType::Value const);
	static NobType::Value GetTypeFromName(wstring const&);

	bool IsOfType(NobType::Value const t) const { return m_value == t; }

	bool IsDefinedType        () const { return !IsOfType(Value::undefined); }
	bool IsUndefinedType      () const { return  IsOfType(Value::undefined); }
	bool IsPipeType           () const { return  IsOfType(Value::pipe); }
	bool IsKnotType           () const { return  IsOfType(Value::knot); }
	bool IsForkType           () const { return  IsOfType(Value::fork); }
	bool IsSynapseType        () const { return  IsOfType(Value::synapse); }
	bool IsNeuronType         () const { return  IsOfType(Value::neuron); }
	bool IsInputLineType      () const { return  IsOfType(Value::inputLine); }
	bool IsOutputLineType     () const { return  IsOfType(Value::outputLine); }
	bool IsInputConnectorType () const { return  IsOfType(Value::inputConnector); }
	bool IsOutputConnectorType() const { return  IsOfType(Value::outputConnector); }

	bool IsIoConnectorType() const
	{
		return IsInputConnectorType() || IsOutputConnectorType();
	}

	bool IsIoLineType() const
	{
		return (m_value == Value::inputLine) || (m_value == Value::outputLine);
	}

	bool IsPosNobType() const
	{
		return IsNeuronType() ||
			   IsIoLineType() ||
			   (m_value == Value::knot) ||
			   (m_value == Value::synapse) ||
			   (m_value == Value::fork);
	}

	Value GetValue() const { return m_value; }

	friend wostream& operator<< (wostream&, NobType const& nobType);

private:
	Value m_value;
};
