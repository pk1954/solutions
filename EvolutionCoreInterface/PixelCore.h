// PixelCore.h
//

#pragma once

#include "pixelPoint.h"

class EvolutionCore;
class EvolutionModelData;
class PixelCoordinates;

class PixelCore
{
public:
	PixelCore
    ( 
        EvolutionCore *, 
        EvolutionModelData *,
		PixelCoordinates *
    );

	~PixelCore( );

	void       SetSelection( PixelPoint const &, PixelPoint const & );
	PixelPoint GetPoiCenter() const;
	void       SetPOI( PixelPoint const pixPos ) const;

private:
	EvolutionCore      * m_pCore;
    EvolutionModelData * m_pModelWork;
	PixelCoordinates   * m_pPixelCoordinates;
};
