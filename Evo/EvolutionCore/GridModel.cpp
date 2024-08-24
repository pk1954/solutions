// GridModel.cpp :
//
// EvoCoreLib

module EvoCoreLib:GridModel;

import std;
import Debug;
import EventInterface;
import :MortalityTable;
import :EvoConfig;
import :GridRect;
import :GridPOI;
import :GridField;
import :Strategy;
import :Individual;
import :Interaction;
import :EvolutionDump;
import :GplIterator;
import :Genome;

using std::endl;
using std::numeric_limits;
using std::wostringstream;

//#ifndef NDEBUG
//    #define CHECK_INDIVIDUALS CheckIndividuals(* this)
//#else
    #define CHECK_INDIVIDUALS
//#endif

ObserverInterface * GridModel::m_pObservers = nullptr;    // GUI call back for display of current model 
EventInterface    * GridModel::m_pEventPOI  = nullptr;
wostringstream    * GridModel::m_pProtocol  = nullptr;

void GridModel::RefreshCache()
{
	m_lFertilizerYield             = EvoConfig::GetConfigValue(EvoConfig::tId::fertilizerYield);
	m_bPassOnEnabled               = EvoConfig::GetConfigValueBool(EvoConfig::tId::passOnEnabled);
	m_bNeighborhoodFoodSensitivity = EvoConfig::GetConfigValueBool(EvoConfig::tId::neighborhoodFoodSensitivity);
	m_enFoodGrowthRate             = GROWTH_RATE (EvoConfig::GetConfigValueShort(EvoConfig::tId::growthRateFood));
	m_enMoveFoodConsumption        = ENERGY_UNITS(EvoConfig::GetConfigValueShort(EvoConfig::tId::energyConsumptionMove));
	m_enCloneFoodConsumption       = ENERGY_UNITS(EvoConfig::GetConfigValueShort(EvoConfig::tId::energyConsumptionClone));
	m_enMarryFoodConsumption       = ENERGY_UNITS(EvoConfig::GetConfigValueShort(EvoConfig::tId::energyConsumptionMarry));
	m_enInteractFoodConsumption    = ENERGY_UNITS(EvoConfig::GetConfigValueShort(EvoConfig::tId::energyConsumptionInteraction));
	m_enMaxFertilizer              = ENERGY_UNITS(EvoConfig::GetConfigValueShort(EvoConfig::tId::maxFertilizer));
	m_enFoodReserve                = ENERGY_UNITS(EvoConfig::GetConfigValueShort(EvoConfig::tId::reserveFood));
	m_enBasicFoodConsumption       = ENERGY_UNITS(EvoConfig::GetConfigValueShort(EvoConfig::tId::energyConsumptionBasicRate));
	m_enMemSizeFoodConsumption     = ENERGY_UNITS(EvoConfig::GetConfigValueShort(EvoConfig::tId::energyConsumptionMemSize));
	Interaction::RefreshCash();
	Individual::RefreshCache();
	Genome::RefreshCash();
}

void GridModel::InitClass
(
	ObserverInterface * const pObservers,
	EventInterface    * const pEvent
)
{
	m_pObservers = pObservers; 
	m_pEventPOI  = pEvent; 
	m_pProtocol  = new wostringstream();
	Genome::InitClass();
	MortalityTable::InitClass();
}

GridModel::GridModel()
    : m_gpList(),
      m_random(),
      m_idCounter(),
 	  m_bPOI(false),
	  m_bPassOnEnabled(true),
	  m_bNeighborhoodFoodSensitivity(true),
	  m_genEvo(0L),
	  m_emptyNeighborSlots(),
	  m_occupiedNeighborSlots(),
	  m_pDisplayList(nullptr),
	  m_gpRun(GP_NULL),     
	  m_gpNext(GP_NULL),     
	  m_gpTarget(GP_NULL),     
	  m_gpPartner(GP_NULL),    
      m_action(Action::Id::undefined),
      m_enBaseConsumption(0_ENERGY_UNITS),
      m_enMaxFertilizer(0_ENERGY_UNITS),
      m_lFertilizerYield(0)
{
	RefreshCache();

	m_aGF.resize(GridDimensions::GridWidthVal());
	for (auto & col: m_aGF)
		col.resize(GridDimensions::GridHeightVal());

    Apply2Grid    // initialization of grid variables which never change after initialization
	(
    	[&](GridPoint const gp)
		{
           getGridField(gp).InitGridFieldStructure(gp);
		}
	); 
}

GridModel::~GridModel()
{
    try
    {
		m_gpList.ResetGpList();
    }
    catch (...)
    {
        exit(1);
    };
}

void GridModel::PrepareComputation() 
{ 
	m_gpTarget .Set2Null();
	m_gpPartner.Set2Null();
	StrategyData::ResetCounters();
	for (auto & ax: (m_ActionCounter))
		ax.fill(ACTION_COUNT(0)); 
}

ACTION_COUNT const GridModel::GetActionCounter(Strategy::Id const strategy, Action::Id const action) const 
{
	unsigned int const uiAction   = static_cast<unsigned int>(action);
	unsigned int const uiStrategy = static_cast<unsigned int>(strategy);

	Assert(uiAction   < Action::COUNT);
	Assert(uiStrategy < Strategy::COUNT);

	return m_ActionCounter[uiAction][uiStrategy];
}

tDisplayMode const GridModel::GetDisplayMode(GridPoint const gp) const 
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

size_t const GridModel::GetGridHeapSize() const
{
	unsigned long long gridFieldSize { sizeof (GridField) };
	unsigned long long gridRowSize   { sizeof(vector< GridField >) + GridDimensions::GridHeightVal() * gridFieldSize };
	unsigned long long gridAreaSize  { GridDimensions::GridWidthVal() * gridRowSize };
	return size_t(gridAreaSize);
}

void CheckIndividuals(GridModel & grid)
{
    int iCount = 0;
 
	Apply2Grid
	(
    	[&](GridPoint const gp)
		{
            if (grid.IsAlive(gp))
               ++ iCount;
		}
	);

	int const iNrOfLivingIndividuals = grid.GetNrOfLivingIndividuals();
    Assert(iCount == iNrOfLivingIndividuals);
}

void GridModel::ResetGrid()
{
    ENERGY_UNITS enFood = ENERGY_UNITS(EvoConfig::GetConfigValueShort(EvoConfig::tId::minFood));
    Apply2Grid([&](GridPoint const gp) { getGridField(gp).ResetGridField(enFood); return false; });
    m_gpList.ResetGpList();
    m_random.Initialize();
}

void GridModel::displayAndWait()
{
	if (m_pObservers)
		m_pObservers->Notify(true);
	if (m_pEventPOI)
		m_pEventPOI->Wait();                // wait for user input to continue
	m_bPOI = GridPOI::IsPoi(m_gpRun);      // POI may have changed in the meantime
}

void GridModel::handleBaseConsumption(GridField & gfRun)
{
	ENERGY_UNITS enBefore   = gfRun.GetEnergy();
	ENERGY_UNITS enBaseCons = m_enBasicFoodConsumption + m_enMemSizeFoodConsumption * gfRun.GetMemSize().GetValue();
	ENERGY_UNITS enAfter;

	gfRun.DecEnergy(enBaseCons);

	enAfter = gfRun.GetEnergy();

	if (m_bPOI)
	{
		* m_pProtocol << L"   base consumption: " 
			          << enBefore  .GetValue() << L" - " 
			          << enBaseCons.GetValue() << L" = " 
			          << enAfter   .GetValue() << endl;
		displayAndWait();
	}
}

void GridModel::inspectNeighborHood()
{
	m_emptyNeighborSlots.Clear();
	m_occupiedNeighborSlots.Clear();

	for (auto gp: GridDimensions::GetNeighbors(m_gpRun))
	{
		(IsAlive(gp) ? m_occupiedNeighborSlots : m_emptyNeighborSlots).AddToList(gp);
	}

	if (m_bPOI)
	{
		* m_pProtocol << L"   neighborhood: " 
			<< m_occupiedNeighborSlots.GetLength() << L" occupied, "
			<< m_emptyNeighborSlots.   GetLength() << L" free" 
			<< endl;
		displayAndWait();
	}

	Assert(m_emptyNeighborSlots.GetLength() + m_occupiedNeighborSlots.GetLength() == GridNeighborhood::GetNrOfNeighbors());
}

Action::Id GridModel::decideOnAction(GridField const & gfRun)
{
	m_options.InitOptions
	(
		gfRun.GetGenome(), 
		m_emptyNeighborSlots   .GetLength() > 0,  // has free space around? bHasFreeSpace, 
		m_occupiedNeighborSlots.GetLength() > 0,  // has neighborbHasNeighbor, 
		gfRun.GetEnergy() 
	);
	
	unsigned int const uiSum  = m_options.GetSumOfValidOptions(gfRun.GetGenome());
	unsigned int const uiVal  = m_random.NextRandomNumberScaledTo(uiSum);
	Action::Id   const action = m_options.SelectAction(gfRun.GetGenome(), uiVal);
		
	if (m_bPOI)
	{
		m_options.DisplayValidOptions(m_pProtocol, gfRun.GetGenome(), uiSum);
		* m_pProtocol << L"   selected action: " << Action::GetName(action) << endl; 
		displayAndWait();
	}

	return action;
}

GridField & GridModel::chooseTarget()
{
	if (m_bPOI)
	{
		* m_pProtocol << L"   free slots: " << m_emptyNeighborSlots.GetLength() << endl;
		m_pDisplayList = & m_emptyNeighborSlots;
		displayAndWait();
	}
	
	if (m_bNeighborhoodFoodSensitivity)
	{
		ENERGY_UNITS enBest = getBestNeighborSlots(m_emptyNeighborSlots);   // consider only neighbor slots with best food stock
		if (m_bPOI)
		{
			* m_pProtocol << L"   best free slots: " << m_emptyNeighborSlots.GetLength() 
				          << L" slots with "      << enBest.GetValue() << L" energy units each" << endl;
			m_emptyNeighborSlots.Apply2All([&](GridPoint const gp)	{ * m_pProtocol << L"   " << gp << endl; });
			displayAndWait();
		}
	}

	m_gpTarget = m_emptyNeighborSlots.GetRandomElement(m_random.NextRandomNumber()); // choose one of them randomly
    Assert(IsDead(m_gpTarget));
	
	if (m_bPOI)
	{
		* m_pProtocol << L"   selected target: " << m_gpTarget << endl;
		m_pDisplayList = nullptr;
		displayAndWait();
	}

	return getGridField(m_gpTarget);
}

GridField & GridModel::choosePartner()
{
	if (m_bPOI)
	{
		* m_pProtocol << L"   partner candidates: " << m_occupiedNeighborSlots.GetLength() << endl;
		m_pDisplayList = & m_occupiedNeighborSlots;
		displayAndWait();
	}

	m_gpPartner = m_occupiedNeighborSlots.GetRandomElement(m_random.NextRandomNumber());
	Assert(IsAlive(m_gpPartner));

	if (m_bPOI)
	{
		printGridPoint(L"   selected partner: ", m_gpPartner);
		m_pDisplayList = nullptr;
		displayAndWait();
	}

	return getGridField(m_gpPartner);
}

void GridModel::actionMove(GridField & gfRun)
{
	GridField & gfTarget = chooseTarget();
	decEnergy(gfRun, m_enMoveFoodConsumption);
	if (gfRun.IsAlive())
	{
		m_gpList.ReplaceGridPointInList(* this, gfRun, gfTarget); 
		gfTarget.MoveIndividual(gfRun);
		if (GridPOI::IsPoi(gfRun.GetGridPoint()))
			GridPOI::SetPoi(m_gpTarget);
	}
	else 
	{
		deleteAndReset(gfRun);
	}
}

void GridModel::actionClone(GridField & gfRun)
{
	GridField & gfTarget = chooseTarget();
	decEnergy(gfRun, m_enCloneFoodConsumption);
	gfTarget.CloneIndividual(++m_idCounter, m_genEvo, m_random, gfRun);
	long lDonationRate = static_cast<long>(gfRun.GetAllele(GeneType::Id::cloneDonation));
	long lParentEnergy = static_cast<long>(gfRun.GetEnergy().GetValue());
	long lDonation = (lDonationRate * lParentEnergy) / numeric_limits<short>::max();
	ENERGY_UNITS enDonation = ENERGY_UNITS(Cast2Short(lDonation));
	if (m_bPOI)
	{
		* m_pProtocol << L"   donation rate : " << lDonationRate << endl;
		* m_pProtocol << L"   current energy: " << lParentEnergy << endl;
	}
	donate(gfRun, gfTarget, enDonation);
	if (gfTarget.IsAlive())
		m_gpList.AddGridPointToList(* this, gfTarget);
	deleteIfDead(gfRun);
}

void GridModel::actionMarry(GridField & gfRun)
{
	GridField & gfPartner = choosePartner();
	GridField & gfTarget  = chooseTarget ();
	decEnergy(gfRun, m_enMarryFoodConsumption);
	gfTarget.BreedIndividual(++m_idCounter, m_genEvo, m_random, gfRun, gfPartner);
	donate(    gfRun, gfTarget,     gfRun.GetEnergy() / 3);   //TODO:  Make variable, Gene?
	donate(gfPartner, gfTarget, gfPartner.GetEnergy() / 3);   //TODO:  Make variable, Gene?
	if (gfTarget.IsAlive())
		m_gpList.AddGridPointToList(* this, gfTarget);
	deleteIfDead(gfPartner);
	deleteIfDead(gfRun);
}

void GridModel::actionInteract(GridField & gfRun)
{
	GridField & gfPartner = choosePartner();
	decEnergy(gfRun, m_enInteractFoodConsumption);
	GridField::Interact(gfRun, gfPartner, m_bPOI ? m_pProtocol : nullptr);
	if (m_gpNext.IsNotNull())
	{
		GridField & gfNext = getGridField(m_gpNext);
		if (gfNext.IsDead())
			m_gpNext = gfNext.GetJuniorGp();
	}
	deleteIfDead(gfPartner);
	deleteIfDead(gfRun);
}

void GridModel::actionPassOn(GridField & gfRun)
{
	if (m_bPOI)
	{
		* m_pProtocol << L"   individual dies naturally at age: " << getAge(gfRun).GetValue() << endl;
		displayAndWait();
	}
	gfRun.PassOn2Child(++m_idCounter, m_genEvo, m_random);
	deleteIfDead(gfRun);
}

void GridModel::actionFertilize(GridField & gfRun)
{
	ENERGY_UNITS const enInvest { ENERGY_UNITS(gfRun.GetAllele(GeneType::Id::fertilInvest)) };
	ENERGY_UNITS const yield    { Cast2Short((enInvest.GetValue() * m_lFertilizerYield) / 100L) };
	ENERGY_UNITS const newValue { std::min(gfRun.GetFertilizer() + yield, m_enMaxFertilizer) };
	gfRun.SetFertilizer(newValue);
	gfRun.DecEnergy(enInvest);
	deleteIfDead(gfRun);
}

void GridModel::actionEat(GridField & gfRun)
{
	ENERGY_UNITS const enWant      = gfRun.GetAppetite();
	ENERGY_UNITS const enStock     = gfRun.GetFoodStock();
	ENERGY_UNITS const enAvailable = enStock - m_enFoodReserve;
	ENERGY_UNITS const enReceive   = ENERGY_UNITS(ClipToMinMax(enAvailable, 0_ENERGY_UNITS, enWant)); 
	if (m_bPOI)
	{
		* m_pProtocol << L"   individual wants: " << setw(4) << enWant.GetValue()          << endl;
		* m_pProtocol << L"   food stock:       " << setw(4) << enStock.GetValue()         << endl;
		* m_pProtocol << L"   reserve:          " << setw(4) << m_enFoodReserve.GetValue() << endl;
		* m_pProtocol << L"   available:        " << setw(4) << enAvailable.GetValue()     << endl;
		* m_pProtocol << L"   individual gets:  " << setw(4) << enReceive.GetValue()       << endl;
		displayAndWait();
	}
	gfRun.IncFoodStock(-enReceive);
	gfRun.IncEnergy   ( enReceive);
	deleteIfDead(gfRun);
}

void GridModel::actionUndefined(GridField & gfRun)
{
	deleteIfDead(gfRun);
}

GridPoint GridModel::ComputeNextGeneration(GridPoint const gpRun)
{
	m_gpRun = gpRun;
	GridField & gfRun = getGridField(m_gpRun);
	Assert(gfRun.IsAlive());

	m_bPOI = GridPOI::IsPoi(m_gpRun);

	if (m_bPOI)
	{
		* m_pProtocol << endl;
		printGridPoint(L"*** start processing POI: ", m_gpRun);
	}

	handleBaseConsumption(gfRun);

	if (m_bPassOnEnabled && MortalityTable::IsTimeToDie(getAge(gfRun), m_random))
	{
		m_action = Action::Id::passOn;
	}
	else
	{
		inspectNeighborHood();
		m_action = decideOnAction(gfRun);
	}

	m_gpNext = gfRun.GetJuniorGp();   

    switch (m_action)
    {
		case Action::Id::eat:       actionEat      (gfRun); break;
		case Action::Id::move:	    actionMove     (gfRun); break;
		case Action::Id::clone:     actionClone    (gfRun); break; 
		case Action::Id::marry:     actionMarry    (gfRun); break;
		case Action::Id::interact:  actionInteract (gfRun); break;
		case Action::Id::passOn:    actionPassOn   (gfRun); break;
		case Action::Id::fertilize: actionFertilize(gfRun); break;
		case Action::Id::undefined:	actionUndefined(gfRun); break;
		default: Assert(false);
    }

	Assert((m_gpNext.IsNull()) || IsAlive(m_gpNext));

	if (gfRun.IsAlive())
		incActionCounter(gfRun.GetStrategyId(), m_action);

	m_bPOI = false;
	m_gpTarget.Set2Null();
	m_gpPartner.Set2Null();

	return m_gpNext;    // may be NULL_VAL
}

void GridModel::EditSetStrategy
(
    GridPoint const gp, 
    PERCENT   const intensity, // percent value
    Strategy::Id    strategy
)
{
	Assert(intensity >= 0_PERCENT);
	if (m_random.NextRandomNumber() < intensity.GetValue() * Random::MAX_VAL / 100ul)
	{
		GridField & gf       = getGridField(gp);
		bool const  bIsAlive = gf.IsAlive();

		if (strategy != Strategy::Id::empty)
		{
			if (strategy == Strategy::Id::random) 
				strategy = static_cast<Strategy::Id>(m_random.NextRandomNumber() % Strategy::COUNT);

			gf.CreateIndividual(++m_idCounter, m_genEvo, strategy);
			if (! bIsAlive)
				m_gpList.AddGridPointToList(* this, gf);
		}
		else if (bIsAlive)
		{
			deleteAndReset(gf);
		}
	}
}

GridPoint const GridModel::FindGridPoint
(
	GridPointBoolFunc const & func, 
	GridRect          const & rect
) const
{
	for (GridCoord y = rect.GetTop();  y <= rect.GetBottom(); ++y)
	for (GridCoord x = rect.GetLeft(); x <= rect.GetRight();  ++x)
	{
		GridPoint gp { x, y };
		if (func(gp))
		{
			return gp;
		}
	}
	return GP_NULL;
}

GridPoint const GridModel::FindGridPointFromId(IND_ID const idSearchedFor) const 
{ 
	return (idSearchedFor.IsNull())
     		? GP_NULL
			: FindGridPoint
			(
				[&](GridPoint const gp) 
				{ 
					return (GetId(gp) == idSearchedFor);
				}, 
				GridDimensions::GridRectFull() 
			);
}

void GridModel::printGridPoint(wchar_t const * text, GridPoint const gp)
{
	* m_pProtocol << text << GetId(gp) << L" at " << gp << endl;
}

void GridModel::decEnergy(GridField & gf, ENERGY_UNITS en)
{
	gf.DecEnergy(en);
	if (m_bPOI)
	{
		* m_pProtocol << L"   consumption:      " << en.GetValue() << endl;
		* m_pProtocol << L"   remaining energy: " << gf.GetEnergy().GetValue() << endl;
		displayAndWait();
	}
}

void GridModel::donate(GridField & gfDonator, GridField & gfReceiver, ENERGY_UNITS enDonation)
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

void GridModel::incActionCounter(Strategy::Id const strategy, Action::Id const action)
{
	unsigned int const uiAction   = static_cast<unsigned int>(action);
	unsigned int const uiStrategy = static_cast<unsigned int>(strategy);

	Assert(uiAction   < Action::COUNT);
	Assert(uiStrategy < Strategy::COUNT);

	++ m_ActionCounter[uiAction][uiStrategy];
}
		
void GridModel::deleteAndReset(GridField & gf)
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

void GridModel::deleteIfDead(GridField & gf)
{
    if (gf.IsDead())
        deleteAndReset(gf);
}

ENERGY_UNITS GridModel::getBestNeighborSlots(GridNeighborhood & list)
{
	ENERGY_UNITS enMaxFoodStock = 0_ENERGY_UNITS;
	list.Apply2All(	     [&](GridPoint const gp) { enMaxFoodStock = std::max(enMaxFoodStock, GetFoodStock(gp)); });
	list.RemoveFromList([&](GridPoint const gp) { return (GetFoodStock(gp) != enMaxFoodStock); });
	return enMaxFoodStock;
}

GridField & GridModel::getGridField(GridPoint const gp)
{
    Assert(IsInGrid(gp));
    return m_aGF[gp.GetXvalue()][gp.GetYvalue()];
};

bool GridModel::isPoi(GridPoint const gp) 
{ 
	return gp.IsNotNull() &&  GridPOI::IsPoi(gp); 
}

EVO_GENERATION GridModel::getAge(GridField const & gf) const 
{
	EVO_GENERATION genBirth = gf.GetGenBirth();
	return genBirth.IsNull() ? EVO_GENERATION::NULL_VAL() : (m_genEvo - genBirth); 
}
