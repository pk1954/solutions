// PixelCore.h
//

#pragma once

#include "pixelPoint.h"

class EvolutionCore;
class EvolutionModelData;
class PixelCoordinates;
class PixelRectSize;

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
	void       SetPOI( PixelPoint const ) const;
	bool       CenterPoi( PixelPoint const ) const;
	void       FitToRect( PixelRectSize const ) const;
	PixelPoint GetNewCenter( PixelPoint const ) const;
	void       SetFieldSize( short const, PixelPoint const );
		
private:
	EvolutionCore      * m_pCore;
    EvolutionModelData * m_pModelWork;
	PixelCoordinates   * m_pPixelCoordinates;
};
