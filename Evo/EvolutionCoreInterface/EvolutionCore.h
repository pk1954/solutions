// EvolutionCore.h
//

module;

#include <iostream>     
#include "MoreTypes.h"
import BoolOp;
#include "GridRect.h"
#include "strategy.h"
#include "ModelInterface.h"
import EvolutionTypes;
import GridDimensions;

class ObserverInterface;
class EventInterface;

class EvolutionCore : public ModelInterface
{
public:

    virtual ~EvolutionCore() { };


// debugging functions

    virtual void DumpGridPointList() const = 0;

// readOnly functions

    virtual int             const GetNrOfLivingIndividuals() const = 0;
    virtual EVO_GENERATION  const GetEvoGenerationNr      () const = 0;
    virtual PERCENT         const GetBrushIntensity       () const = 0;
    virtual tShape          const GetBrushShape           () const = 0;
    virtual tManipulator    const GetBrushManipulator     () const = 0;
    virtual GRID_COORD      const GetBrushSize            () const = 0;
    virtual tBrushMode      const GetBrushMode            () const = 0;
	virtual BYTES           const GetGridHeapSize         () const = 0;

	virtual tDisplayMode    const GetDisplayMode (GridPoint const) const = 0;
	virtual bool            const IsDead         (GridPoint const) const = 0;
	virtual bool            const IsAlive        (GridPoint const) const = 0;
    virtual bool            const IsDefined      (GridPoint const) const = 0;
    virtual IND_ID          const GetId          (GridPoint const) const = 0;
    virtual tOrigin         const GetOrigin      (GridPoint const) const = 0;
    virtual ENERGY_UNITS    const GetEnergy      (GridPoint const) const = 0;
    virtual ENERGY_UNITS    const GetFoodStock   (GridPoint const) const = 0;
    virtual ENERGY_UNITS    const GetFertility   (GridPoint const) const = 0;
    virtual PERCENT         const GetMutRate     (GridPoint const) const = 0;
    virtual ENERGY_UNITS    const GetFertilizer  (GridPoint const) const = 0;
    virtual EVO_GENERATION  const GetGenBirth    (GridPoint const) const = 0;
    virtual EVO_GENERATION  const GetAge         (GridPoint const) const = 0;
    virtual Strategy::Id    const GetStrategyId  (GridPoint const) const = 0;
    virtual MEM_INDEX       const GetMemSize     (GridPoint const) const = 0;
    virtual MEM_INDEX       const GetMemUsed     (GridPoint const) const = 0;
	virtual wchar_t const * const GetOriginName  (GridPoint const) const = 0;
	virtual wchar_t const * const GetStrategyName(GridPoint const) const = 0;

	virtual IND_ID          const GetMemEntry        (GridPoint    const, MEM_INDEX    const) const = 0;
    virtual short           const GetAllele          (GridPoint    const, GeneType::Id const) const = 0;
	virtual	ACTION_COUNT    const GetActionCounter   (Strategy::Id const, Action::Id   const) const = 0;
	virtual GridPoint       const FindGridPointFromId(IND_ID       const)                     const = 0;
	  
	BYTES const GetCoreSize();

// manipulating functions

    virtual bool Compute            () = 0;
    virtual void ResetAll           () = 0;
    virtual void SetBrushManipulator(tManipulator const) = 0;
    virtual void SetBrushShape      (tShape       const) = 0;
    virtual void SetBrushRadius     (GRID_COORD   const) = 0;
    virtual void SetBrushIntensity  (PERCENT      const) = 0;
    virtual void SetBrushMode       (tBrushMode   const) = 0;
    virtual void ModelDoEdit        (GridPoint    const) = 0;
	
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
};
