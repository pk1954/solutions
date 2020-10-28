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
		SSIndexVector indexList   ( sizeOfModel, SSI_NULL );      // indexes into m_copies

		auto dstFromSrc = [&](Shape const * pSrc )
		{ 
			ShapeId idSrc { pSrc->GetId() };
			if ( idSrc.IsNotNull() )
			{
				SelShapesIndex & ssi { indexList[pSrc->GetId().GetValue()] };
				if ( ssi.IsNotNull() )
					return  m_copies[ssi.GetValue()].get();
			}
			return (Shape *)0;
		};

		ShapeId idShapeCopy { ShapeId( Cast2Long(sizeOfModel) ) };

		nmwi.Apply2AllSelected<Shape>
		(
			[&]( Shape & s )
			{
				m_selectedShapeIds.push_back( s.GetId() );
				indexList[s.GetId().GetValue()] = SelShapesIndex(Cast2Int(m_copies.size()) );
				m_copies.push_back( ShallowCopy( s ) );
			}
		);
		for ( UPShape & upShapeDst : m_copies )  // link shapes
		{
			Shape const & shapeSrc { * nmwi.GetShape( upShapeDst->GetId() ) };
			nmwi.GetModel().GetShapes().LinkShape( shapeSrc, dstFromSrc );
			upShapeDst->SetId( idShapeCopy++ );
		}
	}

	virtual void Do( NNetModelWriterInterface & nmwi ) 
	{ 
		nmwi.SelectAllShapes( tBoolOp::opFalse );  // deselect all
		for ( auto & upShape : m_copies )  // add copies (which are selected)
		{
			nmwi.Add2Model( move(upShape) );
		}
		m_copies.clear();
	}

	virtual void Undo( NNetModelWriterInterface & nmwi ) 
	{ 
		nmwi.Apply2AllSelected<Shape>([&](Shape & s) { m_copies.push_back( nmwi.RemoveFromModel<Shape>( s.GetId() ) ); } );
		nmwi.SelectAllShapes( tBoolOp::opFalse );
		for ( auto & idShape : m_selectedShapeIds ) { nmwi.SelectShape( idShape, tBoolOp::opTrue ); };
	}

private:
	vector<UPShape> m_copies;
	vector<ShapeId> m_selectedShapeIds;
};

