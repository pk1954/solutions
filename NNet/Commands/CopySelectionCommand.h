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

	void addMissingKnot(BaseKnot * pBaseKnot, function<Shape * (Shape const *)> const & dstFromSrc)
	{
		if ( ! dstFromSrc(pBaseKnot))
		{
			unique_ptr<Knot> upKnot { make_unique<Knot>(pBaseKnot->GetPosition()) };
			upKnot->SetId( pBaseKnot->GetId() );
			m_indexList[upKnot->GetId().GetValue()] = SelShapesIndex(Cast2Int(m_copies.size()));
			m_copies.push_back(move(upKnot));
		}
	}

	void init( NNetModelWriterInterface & nmwi )
	{ 
		auto dstFromSrc = [&](Shape const * pSrc )
		{ 
			if ( pSrc && IsDefined(pSrc->GetId()) )
			{
				SelShapesIndex & ssi { m_indexList[pSrc->GetId().GetValue()] };
				if ( ssi.IsNotNull() )
					return m_copies[ssi.GetValue()].get();
			}
			return (Shape *)0;
		};

		long const lSizeOfModel { nmwi.GetUPShapes().Size() };
		ShapeId    idShapeCopy  { ShapeId( lSizeOfModel ) };

		m_indexList.resize( lSizeOfModel, SelShapesIndex::NULL_VAL() ); 

		nmwi.GetUPShapes().Apply2AllSelected<Shape>
		(
			[&](Shape & shape)
			{
				m_selectedShapeIds.Add(shape);
				UPShape upShapeCopy { ShallowCopy(shape) };
				m_indexList[upShapeCopy->GetId().GetValue()] = SelShapesIndex(Cast2Int(m_copies.size()));
				m_copies.push_back( move(upShapeCopy) );
			}
		);
		int const iSize { Cast2Int(m_copies.size()) };
		for (int i = 0; i < iSize; ++i) // cannot use range-based loop. m_copies changed in loop.
		{
			Shape * const pShapeSrc { nmwi.GetShape( m_copies[i]->GetId() ) };
			if ( pShapeSrc->IsPipe() )
			{
				Pipe * const pPipeSrc { static_cast<Pipe *>(pShapeSrc) };
				addMissingKnot(pPipeSrc->GetStartKnotPtr(), dstFromSrc);
				addMissingKnot(pPipeSrc->GetEndKnotPtr  (), dstFromSrc);
			}
		}
		for ( UPShape & upShapeDst : m_copies )  // link shapes
		{
			Shape const & shapeSrc { * nmwi.GetShape( upShapeDst->GetId() ) };
			nmwi.GetUPShapes().LinkShape( shapeSrc, dstFromSrc );
			upShapeDst->SetId( idShapeCopy++ );
			if ( upShapeDst->GetShapeType().IsBaseKnotType() )
				upShapeDst->MoveShape( PIPE_WIDTH ); 
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
		nmwi.GetUPShapes().DeselectAllShapes();  
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
			m_copies.push_back(nmwi.GetUPShapes().Pop<Shape>());
		}
		nmwi.GetUPShapes().DeselectAllShapes();
		m_selectedShapeIds.Apply2All([&](ShapeId const &id) { nmwi.SelectShape(id, true); });
	}

private:
	using SelShapesIndex = NamedType <int, struct SelShapesIndex_Parameter>;
	using SSIndexVector  = vector<SelShapesIndex>;

	bool            m_bInitialized     { false };
	int             m_iSizeOfSelection { 0 };
	SSIndexVector   m_indexList        {};     // indices into m_copies
	vector<UPShape> m_copies           {};
	ShapeIdList m_selectedShapeIds {};
};

