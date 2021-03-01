// ShapeAnimation.h
//
// NNetWindows

#pragma once

#include "BaseKnot.h"
#include "ShapePtrList.h"
#include "MicroMeterPointVector.h"
#include "NNetModelReaderInterface.h"
#include "NNetModelCommands.h"
#include "win32_animation.h"

using std::unique_ptr;

template<typename WIN>
class ShapeAnimation
{
public:
	ShapeAnimation
	( 
		HWND                     const   hwnd, 
		NNetModelCommands              & commands,
		NNetModelReaderInterface const & modelReaderInterface
	)
	  : m_hwnd(hwnd),
		m_NNetCommands(commands),
		m_NMRI(modelReaderInterface)
	{
		m_upConnectorAnimation = make_unique<Animation<MicroMeterPointVector, WIN>>
		(
			hwnd, 
			m_umPntVectorRun,
			[&](WIN * const pWin, bool const bTargetReached) 
			{
				pWin->GetShapeAnimation()->setBaseKnots( bTargetReached );
			}
		);
	}

	void setBaseKnots( bool const bTargetReached ) 
	{ 
		m_NNetCommands.SetBaseKnots(m_umPntVectorRun, m_shapesAnimated); 
		//if ( bTargetReached )
		//	SendCommand2Application( IDX_PLAY_SOUND, (LPARAM)L"SNAP_IN_SOUND" ); 
	}

private:

	HWND                       m_hwnd;
	NNetModelCommands         & m_NNetCommands;
	NNetModelReaderInterface const & m_NMRI;
	unique_ptr<ShapeAnimation> m_upConnectorAnimation;
	ShapePtrList<BaseKnot>     m_shapesAnimated;
	MicroMeterPointVector      m_umPntVectorRun;
	MicroMeterPointVector      m_umPntVectorTarget;


	void AlignSelection( )
	{
		m_NNetCommands.RestrictSelection( ShapeType::Value::inputNeuron ); /// TODO: 
		alignedShapes();
		m_upConnectorAnimation->Start( m_umPntVectorRun, m_umPntVectorTarget );
	}

	void alignedShapes( )
	{
		struct AL_PNT
		{
			BaseKnot * pBaseKnot;
			MicroMeter umDist;
		};
		vector<AL_PNT> points;

		m_NMRI.GetUPShapes().Apply2AllSelected<BaseKnot>
		( 
			[&](BaseKnot const & b)
			{ 
				if ( b.IsAnyNeuron() )  // TODO: const_cast find better solution
				{
					BaseKnot * pBaseKnot { const_cast<BaseKnot *>(& b) };
					points.push_back( AL_PNT{ pBaseKnot } );
				}
			} 
		);

		BaseKnot const * pStart { nullptr };
		BaseKnot const * pEnd   { nullptr };

		// find 2 baseknots (START/END) with maximum distance

		MicroMeter maxDist { 0.0_MicroMeter };
		for ( auto & it1 : points )
			for ( auto & it2 : points )
			{
				MicroMeter dist { Distance( it1.pBaseKnot->GetPosition(), it2.pBaseKnot->GetPosition() ) };
				if ( dist > maxDist )
				{
					maxDist = dist;
					pStart = it1.pBaseKnot;
					pEnd   = it2.pBaseKnot;
				}
			}

		// for every other baseknot compute position on line START to END

		MicroMeterLine line      { pStart->GetPosition(), pEnd->GetPosition() };
		MicroMeterLine orthoLine { line.OrthoLine() };
		for ( auto & it : points )
			it.umDist = PointToLine( orthoLine, it.pBaseKnot->GetPosition() );

		// sort baseknots according to position

		sort( points.begin(), points.end(), [](auto & p1, auto & p2){ return p1.umDist < p2.umDist; } );

		// compute tightly packed positions

		float           fNrOfPoints       { Cast2Float(points.size()) };
		MicroMeter      umOriginLength    { line.Length() };
		MicroMeter      umTargetLength    { NEURON_RADIUS * 2.0f * fNrOfPoints };
		MicroMeterPoint umPntPackedSingle { line.GetVector() * (umTargetLength / umOriginLength) / fNrOfPoints };
		MicroMeterPoint umPntTargetCenter { (pEnd->GetPosition() + pStart->GetPosition()) * 0.5f };
		MicroMeterPoint umPntTargetStart  { umPntTargetCenter - umPntPackedSingle * fNrOfPoints * 0.5f };
		MicroMeterPoint umPntTargetEnd    { umPntTargetCenter + umPntPackedSingle * fNrOfPoints * 0.5f };

		// fill animation vectors

		m_umPntVectorTarget.Clear();
		m_umPntVectorRun.Clear();
		m_shapesAnimated.Clear();

		for ( int i = 0; i < points.size(); ++i )
		{
			m_umPntVectorTarget.Add( umPntTargetStart + umPntPackedSingle * static_cast<float>(i) );
			m_shapesAnimated.Add( points[i].pBaseKnot );
			m_umPntVectorRun.Add( points[i].pBaseKnot->GetPosition() );
		}
	}

};