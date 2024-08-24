// GridModel.ixx
//
// EvoCoreLib

export module EvoCoreLib:GridModel;

import Debug;
import Random;
import EventInterface;
import ObserverInterface;
import :ActionOptions;
import :EvolutionTypes;
import :GridNeighborhood;
import :GridDimensions;
import :GridPointList;
import :GridField;
import :GridPoint;
import :GridRect;
import :GridPOI;

using std::array;
using std::vector;
using std::wostringstream;
using std::endl;

using GROWTH_RATE = NamedType<int, struct GROWTH_RATE_Parameter>;

export class GridModel
{
public:

	static void InitClass
	(
		ObserverInterface * const, 
		EventInterface    * const
	);

    GridModel();
    ~GridModel();

	void RefreshCache();

	void ResetGrid ();
    void FoodGrowth();

	void PrepareComputation(); 

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
        Assert(IsInGrid(gp));
        return m_aGF[gp.GetXvalue()][gp.GetYvalue()];
    };

    bool           const IsAlive     (GridPoint const gp) const { return GetGridField(gp).IsAlive     (); }
    bool           const IsDead      (GridPoint const gp) const { return GetGridField(gp).IsDead      (); }
    ENERGY_UNITS   const GetFoodStock(GridPoint const gp) const { return GetGridField(gp).GetFoodStock(); }
    IND_ID         const GetId       (GridPoint const gp) const { return GetGridField(gp).GetId       (); }
    tOrigin        const GetOrigin   (GridPoint const gp) const { return GetGridField(gp).GetOrigin   (); }
    EVO_GENERATION const GetGenBirth (GridPoint const gp) const { return GetGridField(gp).GetGenBirth (); }
    EVO_GENERATION const GetAge      (GridPoint const gp) const { return getAge(GetGridField(gp)); }

    GridPoint    const FindGridPoint(GridPointBoolFunc const &, GridRect const &) const;
	GridPoint    const FindGridPointFromId(IND_ID const)                          const;
	ACTION_COUNT const GetActionCounter(Strategy::Id const, Action::Id const)     const;
	tDisplayMode const GetDisplayMode(GridPoint const)                            const;
	size_t       const GetGridHeapSize()                                          const;

	EVO_GENERATION GetEvoGenerationNr()       const { return m_genEvo; }
    int            GetNrOfLivingIndividuals() const { return m_gpList.GetSize(); }

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

	EVO_GENERATION getAge        (GridField const&) const;
	void           decEnergy     (GridField&, ENERGY_UNITS);
	void           donate        (GridField&, GridField&, ENERGY_UNITS);
    void           deleteAndReset(GridField&);
    void           deleteIfDead  (GridField&);
	void           printGridPoint(wchar_t const *, GridPoint const);
	void           incActionCounter(Strategy::Id const, Action::Id const);
	ENERGY_UNITS   getBestNeighborSlots(GridNeighborhood&);
    GridField&     getGridField(GridPoint const);
	bool           isPoi(GridPoint const);

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

	vector<vector<GridField>> m_aGF;     // 15.000 * 108 byte = 1.620.000 byte
                                              
    GridPointList     m_gpList;            //                            10 byte
    EVO_GENERATION    m_genEvo;            //                             4 byte
    GridNeighborhood  m_emptyNeighborSlots;
    GridNeighborhood  m_occupiedNeighborSlots;
	GridPoint         m_gpTarget;  // target for move, clone and marry
	GridPoint         m_gpPartner; // partner for interaction and marry 
	GridPoint         m_gpNext;
	GridPoint         m_gpRun;
	Action::Id        m_action;
	ENERGY_UNITS      m_enBaseConsumption;
	bool              m_bPOI;
	GridNeighborhood* m_pDisplayList;
	ActionOptions     m_options;
	GROWTH_RATE       m_enFoodGrowthRate;
    ENERGY_UNITS      m_enMoveFoodConsumption;
    ENERGY_UNITS      m_enCloneFoodConsumption;
    ENERGY_UNITS      m_enMarryFoodConsumption;
    ENERGY_UNITS      m_enInteractFoodConsumption;
	ENERGY_UNITS      m_enMaxFertilizer;
	ENERGY_UNITS      m_enFoodReserve;
	ENERGY_UNITS      m_enBasicFoodConsumption;
	ENERGY_UNITS      m_enMemSizeFoodConsumption;
	long              m_lFertilizerYield;
	bool              m_bNeighborhoodFoodSensitivity;
	bool              m_bPassOnEnabled;

	array< array < ACTION_COUNT, Strategy::COUNT>, Action::COUNT > m_ActionCounter;

    // following members are stored here only to be part of grid history.

    Random m_random;                                               //  16 byte
    IND_ID m_idCounter;                                            //   4 byte
     
    // static members

	static ObserverInterface * m_pObservers;    // GUI call back for display of current model 
	static EventInterface    * m_pEventPOI;
	static wostringstream    * m_pProtocol;
};

void CheckIndividuals(GridModel &);
