// grid_model.h : 
//

module;

#include <array> 
#include <vector> 
#include <algorithm>
#include <string>     
#include <sstream>     
#include "random.h"
#include "gpList.h"
#include "gridField.h"
#include "gridPOI.h"
#include "ActionOptions.h"
import GridDimensions;
import EvolutionTypes;

import GridPoint;
import GridRect;

using std::array;
using std::vector;
using std::wostringstream;
using std::endl;

class GridCircle;
class Manipulator;
class EventInterface;
class ObserverInterface;

using GROWTH_RATE = NamedType< int, struct GROWTH_RATE_Parameter >;

class Grid
{
public:

	static void InitClass
	(
		ObserverInterface * const, 
		EventInterface    * const
	);

    Grid();
    ~Grid();

	void RefreshCache();

	void ResetGrid ();
    void FoodGrowth();

	GridPoint ComputeNextGeneration(GridPoint const);

	PERCENT      GetMutRate   (GridPoint const gp) { return getGridField(gp).GetMutRate   (); }
	ENERGY_UNITS GetFertilizer(GridPoint const gp) { return getGridField(gp).GetFertilizer(); }
	ENERGY_UNITS GetFoodStock (GridPoint const gp) { return getGridField(gp).GetFoodStock (); }
	ENERGY_UNITS GetFertility (GridPoint const gp) { return getGridField(gp).GetFertility (); }

	void Apply2MutRate   (GridPoint const gp, PERCENT      const s, ManipulatorFunc m) { getGridField(gp).Apply2MutRate   (s, m); }
	void Apply2Fertilizer(GridPoint const gp, ENERGY_UNITS const s, ManipulatorFunc m) { getGridField(gp).Apply2Fertilizer(s, m); }
	void Apply2FoodStock (GridPoint const gp, ENERGY_UNITS const s, ManipulatorFunc m) { getGridField(gp).Apply2FoodStock (s, m); }
	void Apply2Fertility (GridPoint const gp, ENERGY_UNITS const s, ManipulatorFunc m) { getGridField(gp).Apply2Fertility (s, m); }

	void SetEnergy(GridPoint const gp, ENERGY_UNITS const s) { getGridField(gp).SetEnergy(s); }
	void IncEnergy(GridPoint const gp, ENERGY_UNITS const s) { getGridField(gp).IncEnergy(s); }
    void DecEnergy(GridPoint const gp, ENERGY_UNITS const s) { getGridField(gp).DecEnergy(s); }

    void IncGenNr() { ++m_genEvo; }

    void EditSetStrategy(GridPoint const, PERCENT const, Strategy::Id);

    // GridPoint list operations

    void CheckGridPointList() const { m_gpList.CheckGridPointList(* this); };

    GridPoint const GetOldestGp  () const { return m_gpList.GetOldestGp  (); }
    GridPoint const GetYoungestGp() const { return m_gpList.GetYoungestGp(); }

    GridPoint const GetSeniorGp(GridPoint const gp) const { return GetGridField(gp).GetSeniorGp(); }
    GridPoint const GetJuniorGp(GridPoint const gp) const { return GetGridField(gp).GetJuniorGp(); }

    void SetSeniorGp(GridPoint const gp, GridPoint const gpSenior) { getGridField(gp).SetSeniorGp(gpSenior); }
    void SetJuniorGp(GridPoint const gp, GridPoint const gpJunior) { getGridField(gp).SetJuniorGp(gpJunior); }

    bool ListIsEmpty() const { return m_gpList.ListIsEmpty(); }

    // Query functions 

    GridField const & GetGridField(GridPoint const gp) const
    {
        assert(IsInGrid(gp));
        return m_aGF[gp.GetXvalue()][gp.GetYvalue()];
    };

    bool           const IsAlive     (GridPoint const gp) const { return GetGridField(gp).IsAlive     (); }
    bool           const IsDead      (GridPoint const gp) const { return GetGridField(gp).IsDead      (); }
    ENERGY_UNITS   const GetFoodStock(GridPoint const gp) const { return GetGridField(gp).GetFoodStock(); }
    IND_ID         const GetId       (GridPoint const gp) const { return GetGridField(gp).GetId       (); }
    tOrigin        const GetOrigin   (GridPoint const gp) const { return GetGridField(gp).GetOrigin   (); }
    EVO_GENERATION const GetGenBirth (GridPoint const gp) const { return GetGridField(gp).GetGenBirth (); }
    EVO_GENERATION const GetAge      (GridPoint const gp) const { return getAge(GetGridField(gp)); }

    GridPoint const FindGridPoint(GridPointBoolFunc const &, GridRect const &) const;
	GridPoint const FindGridPointFromId(IND_ID const) const;

	BYTES const GetGridHeapSize() const;

	EVO_GENERATION GetEvoGenerationNr()       const { return m_genEvo; }
    int            GetNrOfLivingIndividuals() const { return m_gpList.GetSize(); }

	void PrepareComputation() 
	{ 
		m_gpTarget .Set2Null();
		m_gpPartner.Set2Null();
		StrategyData::ResetCounters();
		for (auto & ax: (m_ActionCounter))
			ax.fill(ACTION_COUNT(0)); 
	}

	ACTION_COUNT const GetActionCounter(Strategy::Id const strategy, Action::Id const action) const 
	{
		unsigned int const uiAction   = static_cast<unsigned int>(action);
		unsigned int const uiStrategy = static_cast<unsigned int>(strategy);

		assert(uiAction   < Action::COUNT);
		assert(uiStrategy < Strategy::COUNT);

		return m_ActionCounter[uiAction][uiStrategy];
	}

	tDisplayMode const GetDisplayMode(GridPoint const gp) const 
	{ 
		if (GridPOI::IsPoiDefined())
		{
			if (GridPOI::IsPoi(gp))
				return tDisplayMode::POI;

			if (gp == m_gpPartner)
				return tDisplayMode::partner;

			if (gp == m_gpTarget)
				return tDisplayMode::target;

			if (m_pDisplayList && 	m_pDisplayList->Includes(gp))
				return tDisplayMode::neighbor;
		}

		return tDisplayMode::normal; 
	};

	// static functions

	static std::wostringstream * GetProtocolData()
	{
		return m_pProtocol;
	}

	static void ClearProtocolData()
	{
		m_pProtocol->str(std::wstring());
		m_pProtocol->clear();
	}

private:

	void printGridPoint(wchar_t const * text, GridPoint const gp)
	{
		* m_pProtocol << text << GetId(gp) << L" at " << gp << endl;
	}

	void decEnergy(GridField & gf, ENERGY_UNITS en)
	{
		gf.DecEnergy(en);
		if (m_bPOI)
		{
			* m_pProtocol << L"   consumption:      " << en.GetValue() << endl;
			* m_pProtocol << L"   remaining energy: " << gf.GetEnergy().GetValue() << endl;
			displayAndWait();
		}
	}

	void Donate(GridField & gfDonator, GridField & gfReceiver, ENERGY_UNITS enDonation)
	{
		gfDonator.DecEnergy(enDonation);
		gfReceiver.IncEnergy(enDonation);
		if (m_bPOI)
		{
			* m_pProtocol << gfDonator.GetId()  << L" donates " << enDonation.GetValue() << L" units to " << gfReceiver.GetId() << endl;
			* m_pProtocol << gfDonator.GetId()  << L" now has " << gfDonator .GetEnergy().GetValue() << L" units" << endl;
			* m_pProtocol << gfReceiver.GetId() << L" now has " << gfReceiver.GetEnergy().GetValue() << L" units" << endl;
			displayAndWait();
		}
	}

	void incActionCounter(Strategy::Id const strategy, Action::Id const action)
	{
		unsigned int const uiAction   = static_cast<unsigned int>(action);
		unsigned int const uiStrategy = static_cast<unsigned int>(strategy);

		assert(uiAction   < Action::COUNT);
		assert(uiStrategy < Strategy::COUNT);

		++ m_ActionCounter[uiAction][uiStrategy];
	}
		
    void deleteAndReset(GridField & gf)
    {
		if (m_bPOI)
		{
			* m_pProtocol << L"   individual " << gf.GetGridPoint() << L" dies of starvation" << endl;
			displayAndWait();
		}
		m_gpList.DeleteGridPointFromList(* this, gf);
        gf.ResetIndividual();  // zero individual data
		if (GridPOI::IsPoi(gf.GetGridPoint()))
			GridPOI::ClearPoi();
    }

    void deleteIfDead(GridField & gf)
    {
        if (gf.IsDead())
            deleteAndReset(gf);
    }

	ENERGY_UNITS Grid::getBestNeighborSlots(Neighborhood & list)
	{
		ENERGY_UNITS enMaxFoodStock = 0_ENERGY_UNITS;
		list.Apply2All(	     [&](GridPoint const gp) { enMaxFoodStock = std::max(enMaxFoodStock, GetFoodStock(gp)); });
		list.RemoveFromList([&](GridPoint const gp) { return (GetFoodStock(gp) != enMaxFoodStock); });
		return enMaxFoodStock;
	}

    GridField & getGridField(GridPoint const gp)
    {
        assert(IsInGrid(gp));
        return m_aGF[gp.GetXvalue()][gp.GetYvalue()];
    };

	bool isPoi(GridPoint const gp) 
	{ 
		return gp.IsNotNull() &&  GridPOI::IsPoi(gp); 
	}

	EVO_GENERATION getAge(GridField const & gf) const 
	{
		EVO_GENERATION genBirth = gf.GetGenBirth();
		return genBirth.IsNull() ? EVO_GENERATION::NULL_VAL() : (m_genEvo - genBirth); 
	}

	GridField & chooseTarget ();
	GridField & choosePartner();

	void actionMove     (GridField &);
	void actionClone    (GridField &);
	void actionMarry    (GridField &);
	void actionInteract (GridField &);
	void actionPassOn   (GridField &);
	void actionFertilize(GridField &);
	void actionEat      (GridField &);
	void actionUndefined(GridField &);

	void       inspectNeighborHood();
	Action::Id decideOnAction(GridField const &);
	void 	   handleBaseConsumption(GridField &);

	void displayAndWait();

	// member variables

	vector< vector < GridField > > m_aGF;     // 15.000 * 108 byte = 1.620.000 byte
                                              
    GridPointList   m_gpList;                 //                            10 byte
    EVO_GENERATION  m_genEvo;                 //                             4 byte
    Neighborhood    m_emptyNeighborSlots;
    Neighborhood    m_occupiedNeighborSlots;
	GridPoint       m_gpTarget;  // target for move, clone and marry
	GridPoint       m_gpPartner; // partner for interaction and marry 
	GridPoint       m_gpNext;
	GridPoint       m_gpRun;
	Action::Id      m_action;
	ENERGY_UNITS    m_enBaseConsumption;
	bool            m_bPOI;
	Neighborhood  * m_pDisplayList;
	ActionOptions   m_options;
	GROWTH_RATE     m_enFoodGrowthRate;
    ENERGY_UNITS    m_enMoveFoodConsumption;
    ENERGY_UNITS    m_enCloneFoodConsumption;
    ENERGY_UNITS    m_enMarryFoodConsumption;
    ENERGY_UNITS    m_enInteractFoodConsumption;
	ENERGY_UNITS    m_enMaxFertilizer;
	ENERGY_UNITS    m_enFoodReserve;
	ENERGY_UNITS    m_enBasicFoodConsumption;
	ENERGY_UNITS    m_enMemSizeFoodConsumption;
	long            m_lFertilizerYield;
	bool            m_bNeighborhoodFoodSensitivity;
	bool            m_bPassOnEnabled;

	array< array < ACTION_COUNT, Strategy::COUNT>, Action::COUNT > m_ActionCounter;

    // following members are stored here only to be part of grid history.

    Random m_random;                                               //  16 byte
    IND_ID m_idCounter;                                            //   4 byte
     
    // static members

	static ObserverInterface * m_pObservers;    // GUI call back for display of current model 
	static EventInterface    * m_pEventPOI;
	static wostringstream    * m_pProtocol;
};

void CheckIndividuals(Grid &);
