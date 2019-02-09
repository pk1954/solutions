// NamedType.h
//

#pragma once

#include <iostream>
#include <iomanip>

template <typename BASE_TYPE, typename Parameter>
class NamedType
{
public:
    NamedType( ) : value_(0) {}
		
    constexpr explicit NamedType( BASE_TYPE const& value ) : value_(value) {}

    BASE_TYPE      & GetValue()       { return value_; }
	BASE_TYPE const& GetValue() const { return value_; }

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

    NamedType& operator+= (NamedType const& other) { value_ += other.GetValue(); return * this; }
    NamedType& operator-= (NamedType const& other) { value_ -= other.GetValue(); return * this; }
    NamedType& operator%= (NamedType const& other) { value_ %= other.GetValue(); return * this; }

    NamedType& operator*= (int       const& i)     { value_ *= i;                return * this; }
	NamedType& operator/= (int       const& i)     { value_ /= i;                return * this; }

    NamedType  operator+ (NamedType const& other) const { NamedType res( value_ + other.GetValue() ); return res; }
    NamedType  operator- (NamedType const& other) const { NamedType res( value_ - other.GetValue() ); return res; }
    NamedType  operator% (NamedType const& other) const { NamedType res( value_ % other.GetValue() ); return res; }
    int        operator/ (NamedType const& other) const { int       res( value_ / other.GetValue() ); return res; }

    NamedType  operator* (int const& i) const { NamedType res( value_ * i ); return res; }
	NamedType  operator/ (int const& i) const { NamedType res( value_ / i ); return res; }

	NamedType  operator- ()                       const { NamedType res( -value_ ); return res; }

	NamedType  operator++() { ++value_; return * this; }
    NamedType  operator--() { --value_; return * this; }

	NamedType const abs_value() const{ NamedType res(::abs(value_)); return res; }

	static NamedType const NULL_VAL()
	{
		static const NamedType res((std::numeric_limits<BASE_TYPE>::min)());
		return res;
	}

private:
    BASE_TYPE value_;
};

template <typename BASE_TYPE, typename Parameter>
std::wostream & operator<< ( std::wostream & out, NamedType<BASE_TYPE, Parameter> const & param )
{
	out << std::setw( 5 ) << param.GetValue();
    return out;
}
