// ShapeType.h
//
// NNetModel

#pragma once

#include <functional>

using std::function;

class ShapeType
{
public:
	enum class Value
	{
		inputNeuron,
		neuron,
		pipe,
		knot,
		shapeTypeLast = knot,
		undefined
	};

	ShapeType( )
		:	m_value( Value::undefined )
	{}

	ShapeType( Value val )
		:	m_value( val )
	{}

	static void Apply2All( function<void(Value const &)> const & func )
	{
		for ( int i = 0; i <= static_cast<int>(ShapeType::Value::shapeTypeLast); ++i )
			func( static_cast<Value>( i ) );
	}

	static wchar_t          const * GetName( ShapeType::Value const );
	static ShapeType::Value const   GetTypeFromName( wchar_t const * const );

	bool IsPipeType       ( ) const { return m_value == Value::pipe;    }
	bool IsDefinedType    ( ) const { return m_value != Value::undefined;   }
	bool IsUndefinedType  ( ) const { return m_value == Value::undefined;   }
	bool IsKnotType       ( ) const { return m_value == Value::knot;        }
	bool IsNeuronType     ( ) const { return m_value == Value::neuron;      }
	bool IsInputNeuronType( ) const { return m_value == Value::inputNeuron; }

	bool IsAnyNeuronType  ( ) const
	{
		return (m_value == Value::neuron)      || 
			   (m_value == Value::inputNeuron);
	}

	bool IsBaseKnotType( ) const
	{
		return (m_value == Value::knot)        || 
			   (m_value == Value::neuron)      || 
			   (m_value == Value::inputNeuron);
	}

	Value GetValue() const { return m_value; }

private:
	Value m_value;
};

