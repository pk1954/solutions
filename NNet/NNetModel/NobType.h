// NobType.h
//
// NNetModel

#pragma once

#include <assert.h>
#include <functional>
#include <bitset>
#include "debug.h"

using std::function;
using std::wostream;
using std::wstring;
using std::size_t;

class NobType
{
public:
	enum class Value
	{
		inputConnector,
		outputConnector,
		inputNeuron,
		outputNeuron,
		neuron,
		knot,
		pipe,
		undefined,
		nobTypeLast  = pipe,
		nobTypeFirst = inputConnector
	};

	static size_t const NR_OF_NOB_TYPES { static_cast<size_t>(Value::nobTypeLast) + 1 };

	NobType()
		:	m_value(Value::undefined)
	{}

    NobType(Value val)
		:	m_value(val)
	{}

	bool operator==(NobType const & rhs) const = default;

	bool operator!=(NobType const & rhs) const
	{
		return !(m_value == rhs.m_value);
	}

	void Check() const
	{
		AssertLimits<int>((int)m_value, (int)Value::nobTypeFirst, (int)Value::undefined);
	}

	static constexpr void Apply2All(function<void(Value const &)> const & func)
	{
		for (int i = 0; i <= static_cast<int>(NobType::Value::nobTypeLast); ++i)
			func(static_cast<Value>(i));
	}

	static constexpr wstring const GetListOfNobTypes()
	{
		wstring wstrList;
		NobType::Apply2All
		(
			[&](NobType::Value const &t) 
			{ 
				wstrList += NobType::GetName(t) + L" ";
			}
		);
		return wstrList;
	}

	static wstring        GetName(NobType::Value const);
	static NobType::Value GetTypeFromName(wstring const &);

	bool IsPipeType           () const { return m_value == Value::pipe;            }
	bool IsDefinedType        () const { return m_value != Value::undefined;       }
	bool IsUndefinedType      () const { return m_value == Value::undefined;       }
	bool IsKnotType           () const { return m_value == Value::knot;            }
	bool IsNeuronType         () const { return m_value == Value::neuron;          }
	bool IsInputNeuronType    () const { return m_value == Value::inputNeuron;     }
	bool IsOutputNeuronType   () const { return m_value == Value::outputNeuron;    }
	bool IsInputConnectorType () const { return m_value == Value::inputConnector;  }
	bool IsOutputConnectorType() const { return m_value == Value::outputConnector; }

	bool IsIoConnectorType() const
	{
		return IsInputConnectorType() || IsOutputConnectorType();
	}

	bool IsIoNeuronType() const
	{
		return (m_value == Value::inputNeuron) || (m_value == Value::outputNeuron);
	}

	bool IsAnyNeuronType() const
	{
		return (m_value == Value::neuron) || IsIoNeuronType();
	}

	bool IsBaseKnotType() const
	{
		return IsAnyNeuronType() || (m_value == Value::knot);
	}

	Value GetValue() const { return m_value; }

	friend wostream & operator<< (wostream &, NobType const & nobType);

private:
	Value m_value;
};

using std::bitset;

class NobTypeFilter
{
public:
	NobTypeFilter() = default;
	NobTypeFilter(NobType::Value val) : m_bits(bitSet(val)) {}
	NobTypeFilter(const NobTypeFilter& other) : m_bits(other.m_bits) {}

	bool const Any()  const { return m_bits.any();  }
	bool const All()  const { return m_bits.all();  }
	bool const None() const { return m_bits.none(); }

	bool const Test (NobType::Value val) const { return m_bits.test (bitSet(val)); }
	void const Set  (NobType::Value val)       {        m_bits.set  (bitSet(val)); }
	void const Unset(NobType::Value val)       {        m_bits.reset(bitSet(val)); }

private:
	
	static unsigned int bitSet(NobType::Value const val) { return 1 << static_cast<unsigned int>(val); }

	bitset<NobType::NR_OF_NOB_TYPES> m_bits;
};
