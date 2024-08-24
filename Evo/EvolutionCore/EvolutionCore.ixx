// EvoCoreLib.ixx
//
// EvoCoreLib

export module EvoCoreLib:EvolutionCore;

import std;
import BoolOp;
import ObserverInterface;
import EventInterface;
import :Strategy;
import :EvolutionTypes;
import :GridDimensions;
import :GridField;
import :GridModel;
import :GridPoint;
import :GridRect;
import :GridPOI;
import :GridBrush;

export class EvolutionCore
{
public:

	EvolutionCore();
    virtual ~EvolutionCore() = default;

	void CopyModelData(EvolutionCore const * const src)
	{
		* this = * static_cast<EvolutionCore const *>(src);
	}

	// debugging functions

    void DumpGridPointList() const;

// readOnly functions

	int             const GetNrOfLivingIndividuals() const { return m_grid.GetNrOfLivingIndividuals(); }
	EVO_GENERATION  const GetEvoGenerationNr      () const { return m_grid.GetEvoGenerationNr(); }
    PERCENT         const GetBrushIntensity       () const { return m_brush.GetIntensity(); }
    tShape          const GetBrushShape           () const { return m_brush.GetShape(); }
    GridCoord       const GetBrushSize            () const { return m_brush.GetRadius(); }
    tBrushMode      const GetBrushMode            () const { return m_brush.GetBrushMode(); }
	tManipulator    const GetBrushManipulator     () const { return m_brush.GetManipulator(); }
	size_t          const GetGridHeapSize         () const { return m_grid.GetGridHeapSize(); };

	tDisplayMode    const GetDisplayMode (GridPoint const gp) const { return m_grid.GetDisplayMode(gp); };
    bool            const IsDead         (GridPoint const gp) const { return getGridField(gp).IsDead(); }
    bool            const IsAlive        (GridPoint const gp) const { return getGridField(gp).IsAlive(); }
    bool            const IsDefined      (GridPoint const gp) const { return getGridField(gp).IsDefined(); }
    IND_ID          const GetId          (GridPoint const gp) const { return getGridField(gp).GetId(); }
    tOrigin         const GetOrigin      (GridPoint const gp) const { return getGridField(gp).GetOrigin(); }
    ENERGY_UNITS    const GetEnergy      (GridPoint const gp) const { return getGridField(gp).GetEnergy(); }
	ENERGY_UNITS    const GetFoodStock   (GridPoint const gp) const { return getGridField(gp).GetFoodStock(); }
    ENERGY_UNITS    const GetFertility   (GridPoint const gp) const { return getGridField(gp).GetFertility(); }
    ENERGY_UNITS    const GetFertilizer  (GridPoint const gp) const { return getGridField(gp).GetFertilizer(); }
	PERCENT         const GetMutRate     (GridPoint const gp) const { return getGridField(gp).GetMutRate(); }
    EVO_GENERATION  const GetGenBirth    (GridPoint const gp) const { return getGridField(gp).GetGenBirth(); }
	EVO_GENERATION  const GetAge         (GridPoint const gp) const { return m_grid.GetAge(gp); }
    Strategy::Id    const GetStrategyId  (GridPoint const gp) const { return getGridField(gp).GetStrategyId(); }
    MEM_INDEX       const GetMemSize     (GridPoint const gp) const { return getGridField(gp).GetMemSize(); }
    MEM_INDEX       const GetMemUsed     (GridPoint const gp) const { return getGridField(gp).GetMemUsed(); }
	wchar_t const * const GetOriginName  (GridPoint const gp) const { return ::GetOriginName(GetOrigin(gp)); }
	wchar_t const * const GetStrategyName(GridPoint const gp) const { return Strategy::GetName(GetStrategyId(gp)); }

    IND_ID          const GetMemEntry   (GridPoint const gp, MEM_INDEX    const index) const { return getGridField(gp).GetMemEntry(index); }
    short           const GetAllele     (GridPoint const gp, GeneType::Id const gene ) const { return getGenome(gp).GetAllele(gene); }
	ACTION_COUNT const GetActionCounter
	(
		Strategy::Id const strategy,
		Action::Id   const action
	) const
	{
		return m_grid.GetActionCounter(strategy, action);
	}
	virtual GridPoint const FindGridPointFromId(IND_ID const id) const
	{ 
		return m_grid.FindGridPointFromId(id);
	}
	  
	size_t const GetCoreSize();

// manipulating functions

	bool Compute();
	void ResetAll();
	void SetBrushMode       (tBrushMode   const mode ) { m_brush.SetBrushMode  (mode );  }
	void SetBrushManipulator(tManipulator const op   ) { m_brush.SetManipulator(op   );  }
	void SetBrushShape      (tShape       const shape) { m_brush.SetShape      (shape);  }
    void SetBrushRadius     (GridCoord    const rad  ) { m_brush.SetRadius     (rad  );  }
    void SetBrushIntensity  (PERCENT      const perc ) { m_brush.SetIntensity  (perc );  }
    void ModelDoEdit        (GridPoint    const gp   ) { (m_brush)(gp); }
	
// static functions  
    
    static void            InitClass(int const, ObserverInterface * const, EventInterface * const);
    static EvolutionCore * CreateCore();
	static void            DestroyModel(EvolutionCore *);

	static std::wostringstream * GetProtocolData  ();
	static void 	             ClearProtocolData();

	static unsigned int GetNrInteractionsWithKnownCulprit();
    static unsigned int GetNrInteractionsWithUnknownCulprit();
    static unsigned int GetMaxPartnerMemory();
	static bool         IsEnabled(GeneType::Id const);
	static bool         IsEnabled(Action::Id const);

private:
	GridModel m_grid;	
	GridBrush m_brush;

    GridField const & getGridField(GridPoint const gp) const { return m_grid.GetGridField(gp); }
    Genome    const & getGenome   (GridPoint const gp) const { return getGridField(gp).GetGenome(); }
};
