// NamedType.h
//

#pragma once

#include <iostream>
#include <stdlib.h>    // abs
#include <iomanip>

template <typename T, template<typename> class crtpType>
struct crtp
{
    T       & underlying()       { return static_cast< T       & >( * this ); }
    T const & underlying() const { return static_cast< T const & >( * this ); }
private:
    crtp(){}
    friend crtpType<T>;
};

template <typename T, typename Parameter, template<typename> class... Skills>
class NamedType : public Skills<NamedType<T, Parameter, Skills...>>...
{
public:
    NamedType( ) : value_(0) {}
		
    constexpr explicit NamedType( T const& value ) : value_(value) {}

    T      & GetValue()       { return value_; }
	T const& GetValue() const { return value_; }

private:
    T value_;
};

template <typename T>
struct Addable : crtp<T, Addable>
{
    T& operator+=(T const& other)       { this->underlying().GetValue() += other.GetValue(); return this->underlying(); }
    T  operator++()                     { ++this->underlying().GetValue();                   return this->underlying(); }
    T  operator+ (T const& other) const { T res( this->underlying().GetValue() + other.GetValue() ); return res; }
};

template <typename T>
struct Subtractable : crtp<T, Subtractable>
{
    T& operator-=(T const& other)       { this->underlying().GetValue() -= other.GetValue(); return this->underlying(); }
    T  operator--()                     { --this->underlying().GetValue();                   return this->underlying(); }
    T  operator- (T const& other) const { T res( this->underlying().GetValue() - other.GetValue() ); return res; }
    T  operator- ()               const { T res( -this->underlying().GetValue() ); return res; }
};

template <typename T>
struct AbsValue : crtp<T, AbsValue>
{
	T const abs_value() const{ T res( ::abs(this->underlying().GetValue() )); return res; }
};

template <typename T>
struct Comparable : crtp<T, Comparable>
{
    bool const operator== (T const other) const { return this->underlying().GetValue() == other.GetValue(); }
    bool const operator!= (T const other) const { return this->underlying().GetValue() != other.GetValue(); }
    bool const operator<= (T const other) const { return this->underlying().GetValue() <= other.GetValue(); }
    bool const operator<  (T const other) const { return this->underlying().GetValue() <  other.GetValue(); }
    bool const operator>= (T const other) const { return this->underlying().GetValue() >= other.GetValue(); }
    bool const operator>  (T const other) const { return this->underlying().GetValue() >  other.GetValue(); }
};

template <typename T>
struct Multiplicable : crtp<T, Multiplicable>
{
    T& operator*=(int const& i)       { this->underlying().GetValue() *= i; return this->underlying(); }
    T  operator* (int const& i) const { return T(this->underlying().GetValue() * i); }
};

template <typename T>
struct Modulo : crtp<T, Modulo>
{
    T& operator%=(int const& i)           { this->underlying().GetValue() %= i; return this->underlying(); }
    T  operator% (int const& i)     const { return T(this->underlying().GetValue() % i); }
    T& operator%=(T   const& other)       { this->underlying().GetValue() %= other.GetValue(); return this->underlying(); }
    T  operator% (T   const& other) const { T res( this->underlying().GetValue() % other.GetValue() ); return res; }
};

template <typename T>
struct Dividable : crtp<T, Dividable>
{
    T&  operator/=(int const& i)           { this->underlying().GetValue() /= i; return this->underlying(); }
    T   operator/ (int const& i)     const { return T(this->underlying().GetValue() / i); }
    int operator/ (T   const& other) const { int res( this->underlying().GetValue() / other.GetValue() ); return res; }
};

//template <typename T>
//struct Printable : crtp<T, Printable>
//{
//    std::wostream& print( std::wostream& out ) const 
//	{ 
//		return out << this->underlying().GetValue(); 
//	}
//	std::wostream & operator << ( std::wostream & out, T const & param )
//	{
//		return out << this->underlying().GetValue();
//	}
//};
 
//template <typename T, typename Parameter>
//std::wostream & operator << ( std::wostream & out, NamedType<T, Parameter> const& param )
//{
//    return param.print(out);
//}
