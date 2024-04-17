// EvoStatistics.ixx
//
// EvolutionCore

module;

export module EvoStatistics;

import EnumArray;
import EvolutionTypes;
import Strategy;
import GridPoint;
import XArray;

export template <typename T>
class XaCounter : public XArray< T, Strategy::COUNT >
{
public:
    T & operator[] (Strategy::Id strategy) 
	{ 
		return XArray::operator[](static_cast<unsigned int>(strategy));
	}

	void operator= (T const op)
	{
		Apply2XArray([&](T elem) { elem = op; });
	}

    void Add(Strategy::Id strategy, T const op)
    {
		XArray::Add(static_cast<unsigned int>(strategy), op);
	}

    void printGeneLine(TextBuffer * pTextBuf, wchar_t const * const data)
    {
		pTextBuf->nextLine(data);
 		Apply2XArray([&](T elem) { pTextBuf->printNumber(elem); });
    };
};

class EvoStatistics
{
public:
	EvoStatistics::EvoStatistics();
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
