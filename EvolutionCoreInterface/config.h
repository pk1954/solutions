// config.h : 
//

#pragma once

#include <unordered_map>
#include <string>

using namespace std;

class Config
{
public:

    virtual ~Config( ) {};

    static void SetDefaultConfiguration( );
	static void DefineConfigWrapperFunctions( );

    enum class tOnOffAuto
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
        stdMemSize,           // for strategies with memory (e.g. TFT)
        // individual energy 
        initialEnergy,
        stdCapacity,
        extraCapacity,    // size of energy storage increase 
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
        eatEnabled,
        // thresholds for activities
        thresholdClone,             // cloning possible if at least that much energy
        thresholdMarry,             // marriage possible if at least that much energy
        thresholdMove,              // move only if at least that much energy
        thresholdFertilize,         // fertilize only if at least that much energy
        maxEat,                     // eat only if energy not greater than this parameter
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
    };

    static bool  UseHistorySystem( ) { return GetConfigValue( tId::maxGeneration ) > 0; };
    static void  SetConfigValue     ( tId const, long const );
    static long  GetConfigValue     ( tId const);
    static short GetConfigValueShort( tId const );

private:

    typedef long tConfigItem;

    static unordered_map< tId, tConfigItem > m_mapConfigData;

    static void defineConfigId( Config::tId const, wstring const &, long const );
};

