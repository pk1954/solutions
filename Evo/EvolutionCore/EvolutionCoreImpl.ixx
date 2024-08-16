// EvolutionCoreImpl.ixx
//
// EvolutionCore

import std;
import EvolutionCore;
import GridDimensions;
import EvolutionTypes;
import BoolOp;
import GridModel;
import GridPoint;
import GridRect;
import GridPOI;
import GridBrush;

//class EventInterface;
//class ObserverInterface;
//
class EvolutionCoreImpl : public EvolutionCore
{
public:

	EvolutionCoreImpl();
    ~EvolutionCoreImpl();

	// write access

	void EvolutionCoreImpl::CopyModelData(ModelInterface const * const src)
	{
		* this = * static_cast<EvolutionCoreImpl const *>(src);
	}

	virtual bool Compute();
    
	virtual void ResetAll();

	virtual void SetBrushMode       (tBrushMode   const mode ) { m_brush.SetBrushMode  (mode );  }
	virtual void SetBrushManipulator(tManipulator const op   ) { m_brush.SetManipulator(op   );  }
	virtual void SetBrushShape      (tShape       const shape) { m_brush.SetShape      (shape);  }
    virtual void SetBrushRadius     (GridCoord   const rad  ) { m_brush.SetRadius     (rad  );  }
    virtual void SetBrushIntensity  (PERCENT      const perc ) { m_brush.SetIntensity  (perc );  }
    virtual void ModelDoEdit        (GridPoint    const gp   ) { (m_brush)(gp); }

	// read access

	virtual void DumpGridPointList() const;

	virtual EVO_GENERATION  const GetAge        (GridPoint const gp) const { return m_grid.GetAge(gp); }

	virtual PERCENT         const GetMutRate    (GridPoint const gp) const { return getGridField(gp).GetMutRate(); }
	virtual ENERGY_UNITS    const GetFoodStock  (GridPoint const gp) const { return getGridField(gp).GetFoodStock(); }
    virtual ENERGY_UNITS    const GetFertility  (GridPoint const gp) const { return getGridField(gp).GetFertility(); }
    virtual ENERGY_UNITS    const GetFertilizer (GridPoint const gp) const { return getGridField(gp).GetFertilizer(); }
    virtual EVO_GENERATION  const GetGenBirth   (GridPoint const gp) const { return getGridField(gp).GetGenBirth(); }
    virtual Strategy::Id    const GetStrategyId (GridPoint const gp) const { return getGridField(gp).GetStrategyId(); }
    virtual IND_ID          const GetId         (GridPoint const gp) const { return getGridField(gp).GetId(); }
    virtual tOrigin         const GetOrigin     (GridPoint const gp) const { return getGridField(gp).GetOrigin(); }
    virtual ENERGY_UNITS    const GetEnergy     (GridPoint const gp) const { return getGridField(gp).GetEnergy(); }
    virtual MEM_INDEX       const GetMemSize    (GridPoint const gp) const { return getGridField(gp).GetMemSize(); }
    virtual MEM_INDEX       const GetMemUsed    (GridPoint const gp) const { return getGridField(gp).GetMemUsed(); }
    virtual bool            const IsDead        (GridPoint const gp) const { return getGridField(gp).IsDead(); }
    virtual bool            const IsAlive       (GridPoint const gp) const { return getGridField(gp).IsAlive(); }
    virtual bool            const IsDefined     (GridPoint const gp) const { return getGridField(gp).IsDefined(); }

    virtual IND_ID          const GetMemEntry   (GridPoint const gp, MEM_INDEX    const index) const { return getGridField(gp).GetMemEntry(index); }
    virtual short           const GetAllele     (GridPoint const gp, GeneType::Id const gene ) const { return getGenome(gp).GetAllele(gene); }
						    	  
	virtual EVO_GENERATION  const GetEvoGenerationNr() const { return m_grid.GetEvoGenerationNr(); }
	virtual size_t           const GetGridHeapSize   () const { return m_grid.GetGridHeapSize(); };

	virtual tDisplayMode    const GetDisplayMode(GridPoint const gp) const { return m_grid.GetDisplayMode(gp); };

	virtual tManipulator    const GetBrushManipulator() const { return m_brush.GetManipulator(); }
    virtual PERCENT         const GetBrushIntensity  () const { return m_brush.GetIntensity(); }
    virtual tShape          const GetBrushShape      () const { return m_brush.GetShape(); }
    virtual GridCoord      const GetBrushSize       () const { return m_brush.GetRadius(); }
    virtual tBrushMode      const GetBrushMode       () const { return m_brush.GetBrushMode(); }

	virtual int const GetNrOfLivingIndividuals() const { return m_grid.GetNrOfLivingIndividuals(); }

	virtual wchar_t const * const GetOriginName  (GridPoint const gp) const { return ::GetOriginName(GetOrigin(gp)); }
	virtual wchar_t const * const GetStrategyName(GridPoint const gp) const { return Strategy::GetName(GetStrategyId(gp)); }

	virtual	ACTION_COUNT const GetActionCounter
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

private:
	GridModel m_grid;	
	GridBrush m_brush;

    GridField const & getGridField(GridPoint const gp) const { return m_grid.GetGridField(gp); }
    Genome    const & getGenome   (GridPoint const gp) const { return getGridField(gp).GetGenome(); }
};
