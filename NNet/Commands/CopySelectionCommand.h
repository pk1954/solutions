// CopySelectionCommand.h
//
// Commands

#pragma once

#include "Shape.h"
#include "NNetParameters.h"
#include "NNetModelWriterInterface.h"
#include "Command.h"

using std::unique_ptr;
using std::make_unique;

class CopySelectionCommand : public Command
{
public:

	CopySelectionCommand( NNetModelWriterInterface & nmwi )
	{ 
		using SelShapesIndex = NamedType < int, struct SelShapesIndex_Parameter >;
		using SSIndexVector  = vector<SelShapesIndex>;

		static SelShapesIndex const SSI_NULL( SelShapesIndex::NULL_VAL() );

		size_t        sizeOfModel { nmwi.GetSizeOfShapeList() };
		SSIndexVector indexList   ( sizeOfModel, SSI_NULL );      // indexes into m_selectedShapes

		auto dstFromSrc = [&](Shape const * pSrc )
		{ 
			SelShapesIndex & ssi { indexList[pSrc->GetId().GetValue()] };
			return  m_selectedShapes[ssi.GetValue()].get();
		};

		ShapeId idShapeCopy { ShapeId( Cast2Long(sizeOfModel) ) };

		nmwi.Apply2All<Shape>
		(
			[&]( Shape & s )
			{
				if ( s.IsSelected( ) )
				{
					indexList[s.GetId().GetValue()] = SelShapesIndex(Cast2Int(m_selectedShapes.size()) );
					m_selectedShapes.push_back( ShallowCopy( s ) );
				}
			}
		);
		for ( UPShape & upShapeDst : m_selectedShapes )  // link shapes
		{
			Shape const & shapeSrc { * nmwi.GetShape( upShapeDst->GetId() ) };
			nmwi.GetModel().GetShapes().LinkShape( shapeSrc, dstFromSrc );
			upShapeDst->SetId( idShapeCopy++ );
		}
	}

	virtual void Do( NNetModelWriterInterface & nmwi ) 
	{ 
		nmwi.SelectAllShapes( tBoolOp::opFalse );  // deselect all
		for ( auto & upShape : m_selectedShapes )  // add copies (which are selected)
		{
			nmwi.Add2Model( move(upShape) );
		}
	}

	virtual void Undo( NNetModelWriterInterface & nmwi ) 
	{ 
		for ( auto & upShape : m_selectedShapes )    	// disconnect copies
		{
			upShape = move(nmwi.RemoveFromModel<Shape>( upShape->GetId() ));
		};
		nmwi.SelectAllShapes( tBoolOp::opFalse );
		for ( auto & upShape : m_selectedShapes )
		{ 
			if ( upShape )
				upShape->Select( tBoolOp::opTrue ); 
		}
	}

private:
	vector<unique_ptr<Shape>> m_selectedShapes;
};

