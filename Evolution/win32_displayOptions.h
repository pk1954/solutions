// win32_displayOptions.h 
//

#pragma once

#include "win32_baseDialog.h"

class ModelData;
class WorkThread;

// The family of GetIntValueFunctor classes provides functors for access to 
// EvolutionCore functions of return type int

class GetIntValueFunctor
{
public:
    GetIntValueFunctor( ModelData const * pModel ) :
        m_pModelWork( pModel )
    {};
    
    virtual int operator() ( GridPoint const & ) const = 0;

protected:
    ModelData const * m_pModelWork;
};

class GetEmptyFunctor;
class GetFoodStockFunctor;
class GetFertilityFunctor;
class GetMutationRateFunctor;
class GetFertilizerFunctor;

class DspOptWindow : public BaseDialog
{
public:
    DspOptWindow( );
    ~DspOptWindow( );

    void Start( HWND const, WorkThread * const, ModelData const * const );

    GetIntValueFunctor const & GetDisplayFunctor( ) const { return * m_pGetIntValueFunctor; }

    void SetDisplayMode( WORD );  // one of IDM_MUT_RATE/IDM_FERTILITY/IDM_FOOD_STOCK/IDM_FERTILIZER
    void SetIndividualsVisible( );
    BOOL AreIndividualsVisible( ) const;

protected:
    virtual INT_PTR UserProc( UINT const, WPARAM const, LPARAM const );

private:
    DspOptWindow             ( DspOptWindow const & );  // noncopyable class 
    DspOptWindow & operator= ( DspOptWindow const & );  // noncopyable class 

    WorkThread             * m_pWorkThread;
    GetFoodStockFunctor    * m_pGetFoodStockFunctor;
    GetFertilityFunctor    * m_pGetFertilityFunctor;
    GetMutationRateFunctor * m_pGetMutationRateFunctor;
    GetFertilizerFunctor   * m_pGetFertilizerFunctor;
    GetEmptyFunctor        * m_pGetEmptyFunctor;
    GetIntValueFunctor     * m_pGetIntValueFunctor;
};
