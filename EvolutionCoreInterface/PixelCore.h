// PixelCore.h
//

#pragma once

#include "pixelPoint.h"

class EvolutionModelData;
class PixelCoordinates;
class PixelRectSize;

class PixelCore
{
public:
	PixelCore
    ( 
        EvolutionModelData *,
		PixelCoordinates *
    );

	~PixelCore( );

	void       SetSelection( PixelPoint const &, PixelPoint const & );
	PixelPoint GetPoiCenter() const;
	bool       CenterPoi( PixelPoint const ) const;
	void       FitToRect( PixelRectSize const ) const;
	PixelPoint GetNewCenter( PixelPoint const ) const;
	void       SetFieldSize( short const, PixelPoint const );
		
private:
    EvolutionModelData * m_pModelWork;
	PixelCoordinates   * m_pPixelCoordinates;
};
