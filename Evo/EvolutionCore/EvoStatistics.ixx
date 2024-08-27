// EvoStatistics.ixx
//
// EvoCoreLib

export module EvoCoreLib:EvoStatistics;

import EnumArray;
import TextBuffer;
import :EvolutionTypes;
import :EvolutionCore;
import :Strategy;
import :GridPoint;

using std::array;
using std::function;

template <typename T>
void DivNonZero(T & op, T const div)
{
    if (div == 0)
        op = 0;
    else
        op /= div;
}

template <typename T, size_t SIZE>
void DivNonZero(array<T, SIZE> &a, array<T, SIZE> const &div)
{
    for (unsigned int uiRun = 0; uiRun < SIZE; ++uiRun)
        DivNonZero(a[uiRun], div[uiRun]);
}

export template <typename T>
class XaCounter
{
public:
    XaCounter()
    {
        Zero();
    }

    void Zero()
    {
        m_tGeneral = 0;
        m_tArray.fill(0);
    }

    T& General() { return m_tGeneral; }

	T& operator[] (Strategy::Id strategy) 
	{ 
		return m_tArray.at(static_cast<unsigned int>(strategy));
	}

	void operator= (T const op)
	{
		apply2XArray([&](T elem) { elem = op; });
	}

    void Add(Strategy::Id strategy, T const op)
    {
        m_tArray.at(static_cast<unsigned int>(strategy)) += op;
        m_tGeneral += op;
	}

    void DivNonZero(XaCounter const & div)
    {
        ::DivNonZero(m_tGeneral, div.m_tGeneral);
        ::DivNonZero(m_tArray,   div.m_tArray);
    }

    void PrintGeneLine(TextBuffer * pTextBuf, wchar_t const * const data)
    {
		pTextBuf->nextLine(data);
 		apply2XArray([&](T elem) { pTextBuf->printNumber(elem); });
    };

private:

    T                         m_tGeneral;
    array<T, Strategy::COUNT> m_tArray;

    void apply2XArray(function<void (T &)> const & func)
	{
        for (auto &elem : m_tArray)
            func(elem);

		func(m_tGeneral);
	}
};

class EvoStatistics
{
public:
	EvoStatistics();
	virtual ~EvoStatistics();

    void Prepare
	(
		EvolutionCore const &, 
		GridRect      const,
		TextBuffer  * const 
	); 

    void printHeader();
    void printIncidence(EvolutionCore const &);
    void printProbabilities();
    void printGeneStat();
    void printCounter(wchar_t const * const data);
    void printAvAge  (wchar_t const * const data);
    void printAvFood (wchar_t const * const data);
    void printMemory (wchar_t const * const data);
    void printCounters(EvolutionCore const &, Action::Id const action);

private:
	void aquireData(EvolutionCore const &, GridPoint const &);
	void scaleData();
	void scale(float &, float const);

	TextBuffer * m_pTextBuf;

    XaCounter<unsigned int> m_gsCounter;          // counter for strategies and sum counter 
    XaCounter<unsigned int> m_gsAverageAge;       // average age of all individuals

	EnumArray<XaCounter<float>,        Action>   m_XaAction;
	EnumArray<XaCounter<unsigned int>, GeneType> m_XaGenes;
	EnumArray<unsigned int,            Strategy> m_auiMemSize;
};
