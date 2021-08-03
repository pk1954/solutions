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
		closedConnector,
		inputConnector,
		outputConnector,
		inputNeuron,
		outputNeuron,
		neuron,
		knot,
		pipe,
		undefined,
		nobTypeLast  = pipe,
		nobTypeFirst = closedConnector
	};

	static size_t const NR_OF_NOB_TYPES { static_cast<size_t>(Value::nobTypeLast) + 1 };

	NobType()
		:	m_value(Value::undefined)
	{}

	NobType(Value val)
		:	m_value(val)
	{}

	bool const operator==(NobType const & rhs) const
	{
		return m_value == rhs.m_value;
	}

	bool const operator!=(NobType const & rhs) const
	{
		return m_value != rhs.m_value;
	}

	void Check() const
	{
		AssertLimits<int>((int)m_value, (int)Value::nobTypeFirst, (int)Value::undefined);
	}

	static void Apply2All(function<void(Value const &)> const & func)
	{
		for (int i = 0; i <= static_cast<int>(NobType::Value::nobTypeLast); ++i)
			func(static_cast<Value>(i));
	}

	static wstring        const GetName(NobType::Value const);
	static NobType::Value const GetTypeFromName(wstring const &);

	bool const IsPipeType           () const { return m_value == Value::pipe;            }
	bool const IsDefinedType        () const { return m_value != Value::undefined;       }
	bool const IsUndefinedType      () const { return m_value == Value::undefined;       }
	bool const IsKnotType           () const { return m_value == Value::knot;            }
	bool const IsNeuronType         () const { return m_value == Value::neuron;          }
	bool const IsInputNeuronType    () const { return m_value == Value::inputNeuron;     }
	bool const IsOutputNeuronType   () const { return m_value == Value::outputNeuron;    }
	bool const IsInputConnectorType () const { return m_value == Value::inputConnector;  }
	bool const IsOutputConnectorType() const { return m_value == Value::outputConnector; }
	bool const IsClosedConnectorType() const { return m_value == Value::closedConnector; }

	bool const IsIoConnectorType() const
	{
		return IsInputConnectorType() || IsOutputConnectorType();
	}

	bool const IsAnyConnectorType() const
	{
		return IsIoConnectorType() || IsClosedConnectorType();
	}

	bool const IsIoNeuronType() const
	{
		return (m_value == Value::inputNeuron) || (m_value == Value::outputNeuron);
	}

	bool const IsAnyNeuronType() const
	{
		return (m_value == Value::neuron) || IsIoNeuronType();
	}

	bool const IsBaseKnotType() const
	{
		return IsAnyNeuronType() || (m_value == Value::knot);
	}

	Value const GetValue() const { return m_value; }

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
	
	static unsigned int const bitSet(NobType::Value const val) { return 1 << static_cast<unsigned int>(val); }

	bitset<NobType::NR_OF_NOB_TYPES> m_bits;
};
