// ShapeType.h
//
// NNetModel

#pragma once

#include <assert.h>
#include <functional>
#include <bitset>
#include "debug.h"

using std::function;
using std::wostream;
using std::size_t;

class ShapeType
{
public:
	enum class Value
	{
		connector,
		inputNeuron,
		outputNeuron,
		neuron,
		pipe,
		knot,
		undefined,
		shapeTypeLast  = knot,
		shapeTypeFirst = connector
	};

	static size_t const NR_OF_SHAPE_TYPES { static_cast<size_t>(Value::shapeTypeLast) + 1 };

	ShapeType( )
		:	m_value( Value::undefined )
	{}

	ShapeType( Value val )
		:	m_value( val )
	{}

	bool operator==( ShapeType const & rhs ) const
	{
		return m_value == rhs.m_value;
	}

	bool operator!=(ShapeType const & rhs) const
	{
		return m_value != rhs.m_value;
	}

	void Check() const
	{
		AssertLimits<int>( (int)m_value, (int)Value::shapeTypeFirst, (int)Value::undefined );
	}

	static void Apply2All( function<void(Value const &)> const & func )
	{
		for ( int i = 0; i <= static_cast<int>(ShapeType::Value::shapeTypeLast); ++i )
			func( static_cast<Value>( i ) );
	}

	static wchar_t          const * GetName( ShapeType::Value const );
	static ShapeType::Value const   GetTypeFromName( wchar_t const * const );

	bool IsPipeType        () const { return m_value == Value::pipe;         }
	bool IsDefinedType     () const { return m_value != Value::undefined;    }
	bool IsUndefinedType   () const { return m_value == Value::undefined;    }
	bool IsKnotType        () const { return m_value == Value::knot;         }
	bool IsNeuronType      () const { return m_value == Value::neuron;       }
	bool IsInputNeuronType () const { return m_value == Value::inputNeuron;  }
	bool IsOutputNeuronType() const { return m_value == Value::outputNeuron; }
	bool IsConnectorType   () const { return m_value == Value::connector;    }

	bool IsAnyNeuronType() const
	{
		return (m_value == Value::neuron)      || 
			   (m_value == Value::inputNeuron) || 
			   (m_value == Value::outputNeuron);
	}

	bool IsBaseKnotType() const
	{
		return IsAnyNeuronType() || (m_value == Value::knot);
	}

	Value GetValue() const { return m_value; }

	friend wostream & operator<< ( wostream &, ShapeType const & shapeType );

private:
	Value m_value;
};

using std::bitset;

class ShapeTypeFilter
{
public:
	ShapeTypeFilter() = default;
	ShapeTypeFilter(ShapeType::Value val) : m_bits(bitSet(val)) {}
	ShapeTypeFilter(const ShapeTypeFilter& other) : m_bits(other.m_bits) {}

	bool const Any()  const { return m_bits.any();  }
	bool const All()  const { return m_bits.all();  }
	bool const None() const { return m_bits.none(); }

	bool const Test (ShapeType::Value val) const { return m_bits.test (bitSet(val)); }
	void const Set  (ShapeType::Value val)       {        m_bits.set  (bitSet(val)); }
	void const Unset(ShapeType::Value val)       {        m_bits.reset(bitSet(val)); }

private:
	
	static unsigned int const bitSet(ShapeType::Value const val) { return 1 << static_cast<unsigned int>(val); }

	bitset<ShapeType::NR_OF_SHAPE_TYPES> m_bits;
};