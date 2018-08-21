// Manipulator.h : 
//

#pragma once

#include <fstream>
#include <iostream>
#include <functional>
#include <algorithm>  // min/max templates

using namespace std;

enum class tOperator
{
	set,
	max,
	min,
	add,
	subtract,
	mean
};

wchar_t const * const GetOperatorName( tOperator const );

template<typename T>
class Manipulator
{
public:
    virtual ~Manipulator() {};
    virtual T operator() (T const, T const ) = 0;
	virtual tOperator GetOperator( ) = 0;
};

template<typename T>
class Set_Manipulator : public Manipulator<T>
{
public:
	virtual T operator() (T const dst, T const src ) { return src; }
	virtual tOperator GetOperator( ) { return tOperator::set; }
};

template<typename T>
class Max_Manipulator : public Manipulator<T>
{
public:
	virtual T operator() (T const dst, T const src ) { return max(dst, src);  }
	virtual tOperator GetOperator( ) { return tOperator::max; }
};

template<typename T>
class Min_Manipulator : public Manipulator<T>
{
public:
	virtual T operator() (T const dst, T const src ) { return min(dst, src);  }
	virtual tOperator GetOperator( ) { return tOperator::min; }
};

template<typename T>
class Add_Manipulator : public Manipulator<T>
{
public:
	virtual T operator() (T const dst, T const src ) { return (dst + src);  }
	virtual tOperator GetOperator( ) { return tOperator::add; }
};

template<typename T>
class Subt_Manipulator : public Manipulator<T>
{
public:
	virtual T operator() (T const dst, T const src ) { return (dst - src);  }
	virtual tOperator GetOperator( ) { return tOperator::subtract; }
};

template<typename T>
class Mean_Manipulator : public Manipulator<T>
{
public:
	virtual T operator() (T const dst, T const src ) { return (dst + src) / 2; }
	virtual tOperator GetOperator( ) { return tOperator::mean; }
};
