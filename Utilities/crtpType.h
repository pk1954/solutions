// crtpType.h
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
