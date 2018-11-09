// win32_leftColumn.cpp
//

#pragma once

#include <iomanip>     
#include "plannedActivity.h"
#include "win32_shape.h"

class IdentifierShape : public Shape
{
public:
	IdentifierShape
	( 
		Shape       * const pParent,
		TextDisplay &       textDisplay
	) :
		Shape( pParent, textDisplay )
	{}

	void FillBuffer( GridPoint const gp )
	{
		m_textDisplay.Buffer() << L"ID: " << setw( 5 ) << m_textDisplay.Core().GetId( gp );
	}

private:
};

class InfoShape : public Shape
{
public:
	InfoShape
	( 
		Shape       * const pParent,
		TextDisplay &       textDisplay
	) :
		Shape( pParent, textDisplay )
	{}

	void FillBuffer( GridPoint const gp )
	{
		wostringstream & buffer = m_textDisplay.Buffer();
		EvolutionCore  & core   = m_textDisplay.Core();

		buffer << L"En: " << setw( 5 ) << core.GetEnergy( gp )                  << endl;
		buffer << L"Age:" << setw( 5 ) << core.GetAge( gp )                     << endl;
		buffer << L"Or: " << setw( 5 ) << GetOriginName( core.GetOrigin( gp ) ) << endl;

		PlannedActivity plan = core.GetPlan( );
		if ( plan.IsValid( ) )
		{
			if ( (gp == plan.GetActor( )) || (gp == plan.GetPartner( )) )
			{
				buffer << (( gp == plan.GetActor( ) ) ? L"** ACTOR **" : L"** PARTNER **") << endl;
				buffer << L"** "  << GetActionTypeName( plan.GetActionType( ) ) << L" **"  << endl;
				buffer << L"BaseCons: " << plan.GetBaseConsumption( )                      << endl;
			}
		}
	}

private:
};

class LeftColumn : public Shape
{
public:
	LeftColumn
	( 
		Shape * const pParent,
		TextDisplay & textDisplay 
	) :
		Shape( pParent, textDisplay ),
		m_idShape  ( this, textDisplay ),
		m_infoShape( this, textDisplay )
	{ }

	virtual void FillBuffer( GridPoint const gp ) { };  // all text in subshapes

	virtual void PrepareShape( GridPoint const gp )
	{
    	short const sFieldSize = m_textDisplay.GetFieldSize();
		if ( sFieldSize >= ZOOM_LEVEL_1 )
		{
			PixelRectSize const rectSize     = GetShapeSize();
			long          const lShapeWidth  = rectSize.GetWidth();
			long          const lShapeHeight = rectSize.GetHeight();
			long                lYpos        = 0;
			long                lHeight      = lShapeHeight / 10;

			m_idShape.SetShapeRect
			( 
				PixelPoint   (           0, lYpos ),
				PixelRectSize( lShapeWidth, lHeight )
			);
			m_idShape.PrepareShape( gp );
			lYpos += lHeight;

			m_infoShape.SetShapeRect
			( 
				PixelPoint   (           0, lYpos ),
				PixelRectSize( lShapeWidth, lShapeHeight - lYpos)
			);
			m_infoShape.PrepareShape( gp );
		}
	}

	void Draw( GridPoint const gp )
	{
    	short const sFieldSize = m_textDisplay.GetFieldSize();
		if ( sFieldSize >= ZOOM_LEVEL_1 )
		{
			m_idShape.Draw( gp );
			m_infoShape.Draw( gp );
		}
	}

	Shape const * FindShape
	( 
		PixelPoint const pnt, 
		GridPoint  const gp
	) const
	{
		Shape const * pShapeRes;
    	short const   sFieldSize = m_textDisplay.GetFieldSize();
		if ( sFieldSize >= ZOOM_LEVEL_1 )
		{
			pShapeRes = m_idShape.FindShape( pnt, gp );
			if ( pShapeRes != nullptr )
				return pShapeRes;

			pShapeRes = m_infoShape.FindShape( pnt, gp );
			if ( pShapeRes != nullptr )
				return pShapeRes;
		}

		return PointInShape( pnt ) ? this : nullptr;
	}

	IdentifierShape const & GetIdentifierShape() const 
	{
		return m_idShape;
	}

private:
	IdentifierShape m_idShape;
	InfoShape       m_infoShape;
};
