// NamedType.h
//

#pragma once

template <typename T, typename Parameter>

class NamedType
{
public:
    explicit NamedType( ) 
		: value_(0) 
	{}
		
    constexpr explicit NamedType( T const value ) 
		: value_(value) 
	{}

    T const& GetValue() const { return value_; }

    NamedType operator++ () { ++value_; return * this; }
    NamedType operator-- () { --value_; return * this; }

    NamedType const operator- () const { return NamedType( -value_ ); }

    bool const operator== (NamedType const a) const { return value_ == a.value_; }
    bool const operator!= (NamedType const a) const { return value_ != a.value_; }
    bool const operator<= (NamedType const a) const { return value_ <= a.value_; }
    bool const operator<  (NamedType const a) const { return value_ <  a.value_; }
    bool const operator>= (NamedType const a) const { return value_ >= a.value_; }
    bool const operator>  (NamedType const a) const { return value_ >  a.value_; }

	NamedType const operator+= (NamedType const a) { value_ += a.value_; return * this; }
    NamedType const operator-= (NamedType const a) { value_ -= a.value_; return * this; }
    NamedType const operator%= (NamedType const a) { value_ %= a.value_; return * this; }

	NamedType const operator*= (int const i) { value_ *= i; return * this; }
	NamedType const operator/= (int const i) { value_ /= i; return * this; }
    NamedType const operator%= (int const i) { value_ %= i; return * this; }

private:
    T value_;
};
