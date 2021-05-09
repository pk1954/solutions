// CopySelectionCommand.h
//
// Commands

#pragma once

#include "Nob.h"
#include "NNetParameters.h"
#include "NNetModelWriterInterface.h"
#include "Command.h"

using std::unique_ptr;
using std::make_unique;

class CopySelectionCommand : public Command
{
private:

	void addMissingKnot(BaseKnot * pBaseKnot, Nob2NobFunc const & dstFromSrc)
	{
		if ( ! dstFromSrc(pBaseKnot))
		{
			unique_ptr<Knot> upKnot { make_unique<Knot>(pBaseKnot->GetPos()) };
			upKnot->SetId( pBaseKnot->GetId() );
			m_indexList[upKnot->GetId().GetValue()] = SelNobsIndex(Cast2Int(m_copies.size()));
			m_copies.push_back(move(upKnot));
		}
	}

	void init( NNetModelWriterInterface & nmwi )
	{ 
		auto dstFromSrc = [&](Nob const * pSrc )
		{ 
			if ( pSrc && IsDefined(pSrc->GetId()) )
			{
				SelNobsIndex & ssi { m_indexList[pSrc->GetId().GetValue()] };
				if ( ssi.IsNotNull() )
					return m_copies[ssi.GetValue()].get();
			}
			return (Nob *)0;
		};

		long const lSizeOfModel { nmwi.GetUPNobs().Size() };
		NobId    idNobCopy  { NobId( lSizeOfModel ) };

		m_indexList.resize( lSizeOfModel, SelNobsIndex::NULL_VAL() ); 

		nmwi.GetUPNobs().Apply2AllSelected<Nob>
		(
			[&](Nob & nob)
			{
				m_selectedNobIds.Add(nob);
				UPNob upNobCopy { ShallowCopy(nob) };
				m_indexList[upNobCopy->GetId().GetValue()] = SelNobsIndex(Cast2Int(m_copies.size()));
				m_copies.push_back( move(upNobCopy) );
			}
		);
		int const iSize { Cast2Int(m_copies.size()) };
		for (int i = 0; i < iSize; ++i) // cannot use range-based loop. m_copies changed in loop.
		{
			Nob * const pNobSrc { nmwi.GetNob( m_copies[i]->GetId() ) };
			if ( pNobSrc->IsPipe() )
			{
				Pipe * const pPipeSrc { static_cast<Pipe *>(pNobSrc) };
				addMissingKnot(pPipeSrc->GetStartKnotPtr(), dstFromSrc);
				addMissingKnot(pPipeSrc->GetEndKnotPtr  (), dstFromSrc);
			}
		}
		for ( UPNob & upNobDst : m_copies )  // link nobs
		{
			Nob const & nobSrc { * nmwi.GetNob( upNobDst->GetId() ) };
			nmwi.GetUPNobs().LinkNob( nobSrc, dstFromSrc );
			upNobDst->SetId( idNobCopy++ );
			if ( upNobDst->GetNobType().IsBaseKnotType() )
				upNobDst->MoveNob( PIPE_WIDTH ); 
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
		nmwi.GetUPNobs().DeselectAllNobs();  
		for ( int i = 0; i < m_iSizeOfSelection; ++i )
		{
			nmwi.GetUPNobs().Push( move(m_copies.back()) ); // add copies (which are already selected)
			m_copies.pop_back();
		}
		assert( m_copies.empty() );
	}

	virtual void Undo( NNetModelWriterInterface & nmwi ) 
	{ 
		for ( int i = 0; i < m_iSizeOfSelection; ++i )
		{
			m_copies.push_back(nmwi.GetUPNobs().Pop<Nob>());
		}
		nmwi.GetUPNobs().DeselectAllNobs();
		m_selectedNobIds.Apply2All([&](NobId const &id) { nmwi.SelectNob(id, true); });
	}

private:
	using SelNobsIndex = NamedType <int, struct SelNobsIndex_Parameter>;
	using SSIndexVector  = vector<SelNobsIndex>;

	bool            m_bInitialized     { false };
	int             m_iSizeOfSelection { 0 };
	SSIndexVector   m_indexList        {};     // indices into m_copies
	vector<UPNob> m_copies           {};
	NobIdList m_selectedNobIds {};
};

