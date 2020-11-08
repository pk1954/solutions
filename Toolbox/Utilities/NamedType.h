// NamedType.h
//
// Inspired by Jonathan Boccara
// https://www.fluentcpp.com/

#pragma once

#include <iostream>
#include <iomanip>
#include <limits.h>

using std::wostream;
using std::numeric_limits;

template <typename BASE_TYPE, typename Parameter>
class NamedType
{
public:
    NamedType( ) : m_value(0) {}
		
    constexpr NamedType( NamedType const & value ) : m_value(value.GetValue()) {}

    constexpr explicit NamedType( BASE_TYPE const value ) : m_value(value) {}

	constexpr BASE_TYPE const & GetValue() const { return m_value; }

	constexpr NamedType const GetAbs     () const { return NamedType( abs(m_value) ); }
	constexpr BASE_TYPE const GetAbsValue() const { return BASE_TYPE( abs(m_value) ); }

	bool const operator== (NamedType const other) const { return m_value == other.GetValue(); }
    bool const operator!= (NamedType const other) const { return m_value != other.GetValue(); }
    bool const operator<= (NamedType const other) const { return m_value <= other.GetValue(); }
    bool const operator<  (NamedType const other) const { return m_value <  other.GetValue(); }
    bool const operator>= (NamedType const other) const { return m_value >= other.GetValue(); }
    bool const operator>  (NamedType const other) const { return m_value >  other.GetValue(); }

	bool IsZero       ( ) const { return m_value == BASE_TYPE(0); };
	bool IsNotZero    ( ) const { return m_value != BASE_TYPE(0); };
	bool IsPositive   ( ) const { return m_value >  BASE_TYPE(0); };
	bool IsNotPositive( ) const { return m_value <= BASE_TYPE(0); };
	bool IsNegative   ( ) const { return m_value <  BASE_TYPE(0); };
	bool IsNotNegative( ) const { return m_value >= BASE_TYPE(0); };

    NamedType& operator+= (NamedType const other) { m_value += other.GetValue(); return * this; }
    NamedType& operator-= (NamedType const other) { m_value -= other.GetValue(); return * this; }
    NamedType& operator%= (NamedType const other) { m_value %= other.GetValue(); return * this; }

    NamedType& operator*= (BASE_TYPE const i) { m_value *= i; return * this; }
	NamedType& operator/= (BASE_TYPE const i) { m_value /= i; return * this; }

	NamedType  operator- () const { NamedType res { -m_value }; return res; }
	NamedType  operator+ () const { NamedType res { +m_value }; return res; }

	NamedType  operator++() { ++m_value; return * this; }
    NamedType  operator--() { --m_value; return * this; }

	NamedType  operator++(int) { NamedType tmp { *this }; operator++(); return tmp; }
	NamedType  operator--(int) { NamedType tmp { *this }; operator--(); return tmp; }

	friend NamedType operator+ (NamedType const a, NamedType const b )
	{ 
		NamedType res { a };
		res += b; 
		return res; 
	}

	friend NamedType operator- (NamedType const a, NamedType const b )
	{ 
		NamedType res { a };
		res -= b; 
		return res; 
	}

	friend NamedType operator% (NamedType const a, NamedType const b )
	{ 
		NamedType res { a };
		res %= b; 
		return res; 
	}

	friend NamedType operator* (NamedType const a, BASE_TYPE const i )
	{ 
		NamedType res { a };
		res *= i; 
		return res; 
	}

	friend NamedType operator/ (NamedType const a, BASE_TYPE const i )
	{ 
		NamedType res { a };
		res /= i; 
		return res; 
	}

	friend BASE_TYPE operator/ (NamedType const a, NamedType const b )
	{ 
		return BASE_TYPE( a.GetValue() / b.GetValue() );
	}

	friend wostream & operator << ( std::wostream & out, NamedType const & param )
	{
		out << param.GetValue();
		return out;
	}

	static NamedType const MAX_VAL()
	{
		static_assert( std::numeric_limits<BASE_TYPE>::is_specialized, "type has no MAX_VAL"	);
		static NamedType constexpr res { (numeric_limits<BASE_TYPE>::max)() };
		return res;
	}

	static NamedType const NULL_VAL()
	{
		static_assert( std::numeric_limits<BASE_TYPE>::is_specialized, "type has no NULL_VAL"	);
		static NamedType constexpr res { (numeric_limits<BASE_TYPE>::min)() };
		return res;
	}

	static NamedType const ZERO_VAL()
	{
		static NamedType constexpr res { BASE_TYPE(0) };
		return res;
	}

	void Set2Null( ) { * this = NULL_VAL(); }

    bool IsNull   ( ) const { return * this == NULL_VAL(); };
    bool IsNotNull( ) const { return * this != NULL_VAL(); };

private:
    BASE_TYPE m_value;
};
