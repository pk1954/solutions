// NamedType.h
//

#pragma once

#include <iostream>
#include <iomanip>
#include <limits.h>

template <typename BASE_TYPE, typename Parameter>
class NamedType
{
public:
    NamedType( ) : value_(0) {}
		
    constexpr explicit NamedType( BASE_TYPE const value ) : value_(value) {}

	BASE_TYPE const & GetValue() const { return value_; }

    bool const operator== (NamedType const other) const { return value_ == other.GetValue(); }
    bool const operator!= (NamedType const other) const { return value_ != other.GetValue(); }
    bool const operator<= (NamedType const other) const { return value_ <= other.GetValue(); }
    bool const operator<  (NamedType const other) const { return value_ <  other.GetValue(); }
    bool const operator>= (NamedType const other) const { return value_ >= other.GetValue(); }
    bool const operator>  (NamedType const other) const { return value_ >  other.GetValue(); }

	bool IsZero       ( ) const { return value_ == 0; };
	bool IsNotZero    ( ) const { return value_ != 0; };
	bool IsPositive   ( ) const { return value_ >  0; };
	bool IsNotPositive( ) const { return value_ <= 0; };
	bool IsNegative   ( ) const { return value_ <  0; };
	bool IsNotNegative( ) const { return value_ >= 0; };

    NamedType& operator+= (NamedType const other) { value_ += other.GetValue(); return * this; }
    NamedType& operator-= (NamedType const other) { value_ -= other.GetValue(); return * this; }
    NamedType& operator%= (NamedType const other) { value_ %= other.GetValue(); return * this; }

    NamedType& operator*= (int const i) { value_ *= i; return * this; }
	NamedType& operator/= (int const i) { value_ /= i; return * this; }

	NamedType  operator- () const { NamedType res( -value_ ); return res; }
	NamedType  operator+ () const { NamedType res( +value_ ); return res; }

	NamedType  operator++() { ++value_; return * this; }
    NamedType  operator--() { --value_; return * this; }

	NamedType  operator++(int) { NamedType tmp(*this); operator++(); return tmp; }
    NamedType  operator--(int) { NamedType tmp(*this); operator--(); return tmp; }

	static NamedType const NULL_VAL()
	{
		static const NamedType res((std::numeric_limits<BASE_TYPE>::min)());
		return res;
	}

	void Set2Null( ) { * this = NULL_VAL(); }

    bool IsNull   ( ) const { return * this == NULL_VAL(); };
    bool IsNotNull( ) const { return * this != NULL_VAL(); };

private:
    BASE_TYPE value_;
};

template <typename BASE_TYPE, typename Parameter>
auto operator+ (NamedType< BASE_TYPE, Parameter > const a, NamedType< BASE_TYPE, Parameter > const b )
{ 
	auto res(a);
	res += b; 
	return res; 
}

template <typename BASE_TYPE, typename Parameter>
auto operator- (NamedType< BASE_TYPE, Parameter > const a, NamedType< BASE_TYPE, Parameter > const b )
{ 
	auto res(a);
	res -= b; 
	return res; 
}

template <typename BASE_TYPE, typename Parameter>
auto operator% (NamedType< BASE_TYPE, Parameter > const a, NamedType< BASE_TYPE, Parameter > const b )
{ 
	auto res(a);
	res %= b; 
	return res; 
}

template <typename BASE_TYPE, typename Parameter>
BASE_TYPE operator/ (NamedType< BASE_TYPE, Parameter > const a, NamedType< BASE_TYPE, Parameter > const b )
{ 
	return a.GetValue() / b.GetValue();
}

template <typename BASE_TYPE, typename Parameter>
auto operator* (NamedType< BASE_TYPE, Parameter > const a, int const i )
{ 
	auto res(a);
	res *= i; 
	return res; 
}

template <typename BASE_TYPE, typename Parameter>
auto operator/ (NamedType< BASE_TYPE, Parameter > const a, int const i )
{ 
	auto res(a);
	res /= i; 
	return res; 
}

template <typename BASE_TYPE, typename Parameter>
auto const abs_value(NamedType< BASE_TYPE, Parameter > const a) 
{ 
	NamedType< BASE_TYPE, Parameter > res(::abs(a.GetValue())); 
	return res; 
}

template <typename BASE_TYPE, typename Parameter>
std::wostream & operator<< ( std::wostream & out, NamedType<BASE_TYPE, Parameter> const & param )
{
	out << std::setw( 5 ) << param.GetValue();
    return out;
}
