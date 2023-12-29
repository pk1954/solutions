/// HistoryTest.cpp : Defines the entry point for the console application.
//


#include <cassert>
#include <iostream>

import Version;
import Int24;
import Symtab;
import ErrHndl;
import ModelData;
import HistoryIterator;
import HistorySystem;

using std::string;
using std::wstring;
using std::wcout;
using std::endl;

class HistTestModelData: public ModelData
{
public:
	HistTestModelData(int iCounter): 
		m_iDataTest(iCounter),
		m_iDataApp(0)
	{ }

	virtual void Compute()
	{

	}
	virtual size_t GetModelSize() const
	{
		return sizeof(HistTestModelData);
	}

	virtual void CopyFrom(ModelData const * const src)
    {
        * this = * static_cast< HistTestModelData const * const >(src);
    }

    virtual void OnAppCommand(GenerationCmd const cmd)
    {
//		wcout << endl << L"OnAppCommand(" << cmd.GetCommand() << L", " << cmd.GetParam() << L")" << endl;
	}

private:
	int m_iDataTest;
	int m_iDataApp;
};

class HistTestModelFactory: public ModelFactory
{
public:
	virtual ModelData * CreateModelData() const 
	{
		return new HistTestModelData(0);  // TODO: is 0 ok???
	}
};

void showHistorySlots(HistorySystem * const pHistorySys)
{
	HistoryIterator iter(pHistorySys->GetHistoryCache()); 

    for (HistSlotNr slotNr = iter.Set2Oldest(); slotNr.IsNotNull(); slotNr = iter.Set2Junior())
        wcout << iter.GetCurrentGeneration() << L" ";

	wcout << endl;
}

void gotoGeneration(HistorySystem * const pHistorySys, HistGeneration const histGenDemanded)
{
	while(histGenDemanded != pHistorySys->GetCurrentGeneration())
	{
		if (histGenDemanded > pHistorySys->GetYoungestGeneration())
		{
			pHistorySys->CreateAppCommand(GenerationCmd::ApplicationCmd(static_cast<GenerationCmd::Id>(42), 0));
		}
		else
		{
	    	pHistorySys->ApproachHistGen(histGenDemanded);
		}
    	wcout << L"Generation: " << histGenDemanded << L" - " << pHistorySys->GetCurrentGeneration() << L" Slots: ";
		showHistorySlots(pHistorySys);
	}
}

void DoTest()
{
	static long constexpr NR_OF_SLOTS {10};

	HistorySystem      * pHistorySys = HistorySystem::CreateHistorySystem();
	HistTestModelFactory modelFactory;
	HistTestModelData    modelData(0);
	HistGeneration       histGenDemanded;

	pHistorySys->StartHistorySystem
	(
		1000,           // # of generations
		NR_OF_SLOTS,    // # of cache slots
		1024ull * 1024 * 1024 * 16,
		& modelFactory
	);

	wcout << L"*** Create " << NR_OF_SLOTS << L" history slots" << endl;

	for (long i = 1; i < NR_OF_SLOTS; ++i)
		pHistorySys->AddHistorySlot();

	assert(pHistorySys->GetNrOfHistCacheSlots().GetValue() == NR_OF_SLOTS);

	wcout << endl << L"*** Iterate thru generations" << endl << endl;

	for (histGenDemanded = 1; histGenDemanded < 30; ++histGenDemanded)
		gotoGeneration(pHistorySys, histGenDemanded);

	wcout << endl << L"*** Now backwards" << endl << endl;

	for (histGenDemanded = 28; histGenDemanded >= 0; --histGenDemanded)
		gotoGeneration(pHistorySys, histGenDemanded);

	wcout << endl << L"*** HistoryTest finished" << endl;
}

class WrapHistoryTest_1 : public Script_Functor
{
public:
    virtual void operator() (Script & script) const
    {
		try
		{
			DoTest();
		}
		catch (...)
		{
			wcout << endl << L"+++ Error in HistoryTest" << endl;
		}
    }
};

int _tmain(int argc, char * argv[])
{
	wcout << VER_PRODUCTNAME_STR << L" " << VER_FILE_DESCRIPTION_STR << endl;
	wcout << L"Build at " << __DATE__ << L" " << __TIME__ << endl;

	ScriptErrorHandler::ScrSetOutputStream(& wcout);
	DEF_FUNC(HistoryTest_1);

    wstring wstrInputFile = L"Test_1.in";

	for (int iCount = 1; iCount < argc; iCount++)
    {
        string strCmd(argv[iCount]);

		if ((strCmd.find(".in") != string::npos) || (strCmd.find(".IN") != string::npos)) 
		{
			wstrInputFile.assign(strCmd.begin(), strCmd.end()); 
		}
    }

	Script::ProcessScript(wstrInputFile);

	return 0;
}
