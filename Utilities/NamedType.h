// NamedType.h
//

#pragma once

template <typename T, template<typename> class crtpType>
struct crtp
{
    T       & underlying()       { return static_cast< T       & >( * this ); }
    T const & underlying() const { return static_cast< T const & >( * this ); }
private:
    crtp(){}
    friend crtpType<T>;
};

template <typename T>
struct Addable : crtp<T, Addable>
{
    T& operator+=(T const& other)       { this->underlying().GetValue() += other.GetValue(); return this->underlying(); }
    T  operator+ (T const& other)       { return T(this->underlying().GetValue() + other.GetValue()); }
    T  operator+ (T const& other) const { return T(this->underlying().GetValue() + other.GetValue()); }
    T  operator+ ()               const { T res( +this->underlying().GetValue() ); return res; }
};

template <typename T>
struct Subtractable : crtp<T, Subtractable>
{
    T& operator-=(T const& other)       { this->underlying().GetValue() -= other.GetValue(); return this->underlying(); }
    T  operator- (T const& other)       { return T(this->underlying().GetValue() - other.GetValue()); }
    T  operator- (T const& other) const { return T(this->underlying().GetValue() - other.GetValue()); }
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

template <typename T, typename Parameter, template<typename> class... Skills>
class NamedType : public Skills<NamedType<T, Parameter, Skills...>>...
{
public:
    explicit NamedType( ) : value_(0) {}
		
    constexpr explicit NamedType( T const& value ) : value_(value) {}

    T      & GetValue()       { return value_; }
	T const& GetValue() const { return value_; }

    NamedType operator++ () { ++value_; return * this; }
    NamedType operator-- () { --value_; return * this; }

    NamedType const operator%= (NamedType const a) { value_ %= a.value_; return * this; }

	NamedType const operator*= (int const i) { value_ *= i; return * this; }
	NamedType const operator/= (int const i) { value_ /= i; return * this; }
    NamedType const operator%= (int const i) { value_ %= i; return * this; }

private:
    T value_;
};
