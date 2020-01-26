// PixelCore.h
//

#pragma once

#include "PixelTypes.h"

class PixelCoordinates;
class EvolutionCore;

class PixelCore
{
public:
	PixelCore
    ( 
        EvolutionCore *,
		PixelCoordinates *
    );

	~PixelCore( );

	void       SetSelection( PixelPoint const, PixelPoint const );
	PixelPoint GetPoiCenter() const;
	bool       CenterPoi( PixelPoint const ) const;
		
private:
    EvolutionCore    * m_pCore;
	PixelCoordinates * m_pPixelCoordinates;
};
