// EnumArray.h : 
//
// Utilities

#pragma once

#include <functional>
#include <array>

using std::array;
using std::function;

// EnumArray: template for classes providing arrays over enum
// Requirements:
//  - base class T has a member enum class with name "Id"
//  - the enum class "Id" has a member "count" as last element
//  - the elements of "Id" start with 0 and are consecutive 

template <typename T, typename INDEX_TYPE>
class EnumArray
{
public:
	T Sum()
	{
		T sum = static_cast<T>(0);
		for (auto & elem : m_tArray)
			sum += elem;
		return sum;
	}

    T const & operator[] (typename INDEX_TYPE::Id index) const
	{ 
		return m_tArray.at(static_cast<unsigned short>(index)); 
	}

	T & operator[] (typename INDEX_TYPE::Id index)
	{ 
		return m_tArray.at(static_cast<unsigned short>(index)); 
	}

	T * begin()	{ return m_tArray.begin(); }
	T * end()	{ return m_tArray.end(); }

	void operator= (T const val)
	{
        for (T & elem : m_tArray)
		{
            elem = val;
		}
	}

	void Apply2All(function<void(T &)> const & func)
	{
        for (T & elem : m_tArray)
		{
            func(elem);
		}
	}

private:
	static int constexpr COUNT = static_cast<int>(INDEX_TYPE::Id::count);

    array <T, COUNT> m_tArray;
};
