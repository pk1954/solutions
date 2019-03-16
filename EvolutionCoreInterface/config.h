// config.h : 
//

#pragma once

#include <unordered_map>
#include <string>
#include "BoolOp.h"
#include "util.h"

class Config
{
public:

    virtual ~Config( ) {};

    static void SetDefaultConfiguration( );
	static void DefineConfigWrapperFunctions( );

    enum class tOnOffAuto : long
    {
        on,
        off,
        automatic
    };

    enum class tId : unsigned short
    {
        // general
        maxGeneration,        // Maximum number of generations, -1 means: no history system 
        nrOfHistorySlots,     // Number of history slots (if enough memory)
        historyDisplay,       // MODE_ON/OFF/AUTO
        miniGridDisplay,      // MODE_ON/OFF/AUTO
		showGridPointCoords,  // show coordinates of grid points on/off
        stdMemSize,           // for strategies with memory (e.g. TFT)
        // individual energy 
        initialEnergy,
        stdCapacity,
        extraCapacity,        // size of energy storage increase 
        // abilities
        neighborhoodFoodSensitivity, // sensitivity for energy level of neighbor slots
        // energy consumption
        energyConsumptionBasicRate,
        energyConsumptionMove,
        energyConsumptionClone,
        energyConsumptionMarry,
        energyConsumptionInteraction,
        // energy consuption
        energyConsumptionMemSize,           // consumption per memory slot
        energyConsumptionextraCapacity,     // consumption per energy storage increase 
        // enable/disable activities
        moveEnabled,
        cloneEnabled,
        marryEnabled,
        interactEnabled,
        fertilizeEnabled,
        passOnEnabled,
        eatEnabled,
        // thresholds for activities
        thresholdClone,       // cloning possible if at least that much energy
        thresholdMarry,       // marriage possible if at least that much energy
        thresholdMove,        // move only if at least that much energy
        thresholdFert,        // fertilize only if at least that much energy
        maxEat,               // eat only if energy not greater than this parameter
        // food
        initialFoodStock,     // initial food stock per slot
        growthRateFood,       // maximum growth rate in percent
        minFood,		      // minimum initial food stock per slot
        maxFood,		      // maximum food stock per slot
        defaultAppetite,      // default value for appetite (nr of energy units per meal)
        defaultFertilInvest,  // default value for investment in fertilizer
        reserveFood,		  // food cannot go below this level
        fertilizerYield,      // fertilizer return on invest as factor in %
        maxFertilizer,        // maximum absolute fertilizer value, higher values will be clipped
        // interaction
        interactionPayOff_R,  // Beide kooperieren und fahren das gute Ergebnis R (Reward) ein. 
        interactionPayOff_S,  // Der Spieler wurde betrogen, er bekommt S (Suckers Payoff). 
        interactionPayOff_T,  // Der Spieler hat den anderen ausgenutzt und erhält T (Temptation).
        interactionPayOff_P,  // Gegenseitige Defektion führt nur zu P (Penalty). 
		// misc
		nrOfNeighbors,        // 6 neighbors: hexagon grid, 4 or 8 neighbors: rectangle grid
		stripMode,            // 1: strip mode on, 0: strip mode off - see D3D
		dimmMode,             // 1: dimm mode on, 0: dimm mode off - see DrawFrame
		generationDelay       // initial delay between generations in ms
    };

	inline static long GetConfigValue( tId const id )
	{
		return m_mapConfigData.at( id );
	}

	inline static short GetConfigValueShort( tId const id )
	{
		return CastToShort( GetConfigValue( id ) );
	}

	inline static bool GetConfigValueBool( tId const id )
	{
		return GetConfigValue( id ) != 0;
	}

	inline static tBoolOp GetConfigValueBoolOp( tId const id )
	{
		long const lValue = GetConfigValue( id );
		return (lValue == 0) ? tBoolOp::opFalse : tBoolOp::opTrue;
	}

	inline static tOnOffAuto GetConfigValueOnOffAuto( tId const id )
	{
		long const lValue = GetConfigValue( id );
		return static_cast<tOnOffAuto>(lValue);
	}

    static void SetConfigValue       ( tId const, long const );
	static bool SetConfigValueBoolOp ( tId const, tBoolOp const );
    static bool UseHistorySystem( )  { return GetConfigValue( tId::maxGeneration ) > 0; };

private:

    using tConfigItem = long;

    static std::unordered_map< tId, tConfigItem > m_mapConfigData;

    static void defineConfigId( Config::tId const, std::wstring const &, long const );
};

