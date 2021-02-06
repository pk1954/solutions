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
private:
	void init( NNetModelWriterInterface & nmwi )
	{ 
		using SelShapesIndex = NamedType < int, struct SelShapesIndex_Parameter >;
		using SSIndexVector  = vector<SelShapesIndex>;

		static SelShapesIndex const SSI_NULL( SelShapesIndex::NULL_VAL() );

		long          lSizeOfModel { nmwi.GetUPShapes().Size() };
		SSIndexVector indexList( lSizeOfModel, SSI_NULL );      // indices into m_copies

		auto dstFromSrc = [&](Shape const * pSrc )
		{ 
			ShapeId idSrc { pSrc->GetId() };
			if ( IsDefined(idSrc) )
			{
				SelShapesIndex & ssi { indexList[pSrc->GetId().GetValue()] };
				if ( ssi.IsNotNull() )
					return  m_copies[ssi.GetValue()].get();
			}
			return (Shape *)0;
		};

		ShapeId idShapeCopy { ShapeId( lSizeOfModel ) };

		nmwi.GetUPShapes().Apply2AllSelected<Shape>
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
			nmwi.GetUPShapes().LinkShape( shapeSrc, dstFromSrc );
			upShapeDst->SetId( idShapeCopy++ );
		}
		m_iSizeOfSelection = Cast2Int(m_copies.size());
	}

public:
	virtual void Do( NNetModelWriterInterface & nmwi ) 
	{ 
		if ( ! m_bInitialized )
		{
			init( nmwi );
			m_bInitialized = true;
		}
		nmwi.GetUPShapes().SelectAllShapes( tBoolOp::opFalse );  // deselect all
		for ( int i = 0; i < m_iSizeOfSelection; ++i )
		{
			nmwi.GetUPShapes().Push( move(m_copies.back()) ); // add copies (which are already selected)
			m_copies.pop_back();
		}
		assert( m_copies.empty() );
	}

	virtual void Undo( NNetModelWriterInterface & nmwi ) 
	{ 
		for ( int i = 0; i < m_iSizeOfSelection; ++i )
		{
			m_copies.push_back(nmwi.GetUPShapes().Pop<Shape>( ));
		}
		nmwi.GetUPShapes().SelectAllShapes( tBoolOp::opFalse );
		for ( auto & idShape : m_selectedShapeIds ) 
		{ 
			nmwi.SelectShape( idShape, tBoolOp::opTrue ); 
		}
	}

private:
	int             m_iSizeOfSelection { 0 };
	vector<UPShape> m_copies;
	vector<ShapeId> m_selectedShapeIds;
	bool            m_bInitialized { false };
};

