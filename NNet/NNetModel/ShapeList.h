// ShapeList.h
//
// NNetModel

#pragma once

#include <vector>
#include "Shape.h"
#include "Knot.h"
#include "Pipe.h"
#include "BaseKnot.h"
#include "Neuron.h"
#include "InputNeuron.h"
#include "Connections.h"
#include "ShapeErrorHandler.h"

using std::vector;
using std::unique_ptr;
using std::make_unique;

class ShapeList
{
public:

	ShapeList() { }
	ShapeList( const ShapeList & );
	~ShapeList();

	ShapeList & operator=  ( ShapeList const & );
	bool        operator== ( ShapeList const & ) const;

	long    const Size          ( )                  const { return Cast2Long( m_list.size() ); }
	ShapeId const IdNewSlot     ( )	                 const { return ShapeId( Cast2Long(m_list.size()) ); }
	bool    const IsShapeDefined( ShapeId const id ) const { return GetAt( id ) == nullptr; }
	bool    const IsValidShapeId( ShapeId const id ) const { return (0 <= id.GetValue()) && (id.GetValue() < Size()); }
	bool    const IsEmptySlot   ( ShapeId const id ) const { return GetAt( id ) == nullptr; }
	Shape * const Front         ( )                  const { return   m_list[0].get(); }
	Shape * const GetAt         ( ShapeId const id ) const { return   m_list[id.GetValue()].get();	}
	Shape       & GetRef        ( ShapeId const id )       { return * m_list[id.GetValue()]; }
	void          Resize        ( long    const nr )       { m_list.resize( nr );	}
	void          Clear         ( )                        { m_list.clear( ); }
	void          Increase      ( long    const nr )       { m_list.resize( m_list.size() + nr ); }

	void                 SetErrorHandler  ( ShapeErrorHandler * const );
	void                 SelectAllShapes  ( tBoolOp const );
	Shape *        const RemoveShape      ( ShapeId const );	
	Shape *        const ReplaceShape     ( ShapeId const, UPShape );	
	void                 SetShape2Slot    ( ShapeId const, UPShape );	 // only for special situations
	void                 Push             ( UPShape );
	void                 CheckShapeList   ( )                                                           const;
	void                 Dump             ( )                                                           const;
	void                 LinkShape        ( Shape const &, function< Shape * (Shape const *)> const & ) const;
	MicroMeterRect const EnclosingRect    ( )                                                           const;
	bool           const AnyShapesSelected( )                                                           const;
	void                 CallErrorHandler ( ShapeId const )                                             const;
	ShapeId        const FindShapeAt      ( MicroMeterPoint const, ShapeCrit const & )                  const;
	bool           const Apply2AllB       (                        ShapeCrit const & )                  const;
	void                 Apply2All        ( function<void(Shape const &)> const & )                     const;
	void                 Apply2All        ( function<void(Shape       &)> const & );
	void                 Append           ( ShapeList & );

	template <Shape_t T>
	unique_ptr<T> Pop( )
	{
		unique_ptr<T> upT { unique_ptr<T>( static_cast<T*>(m_list.back().release()) ) };
		upT->DecCounter();
		m_list.pop_back( );
		return move( upT );
	}

	template <Shape_t T>    // const version
	void Apply2All( function<void(T const &)> const & func ) const
	{
		for ( auto & it : m_list )
		{ 
			if ( HasType<T>( * it.get() ) ) 
				func( static_cast<T const &>( * it.get() ) );
		};
	}                        

	template <Shape_t T>
	bool Apply2AllB( function<bool(T &)> const & func ) const
	{
		bool bResult { false };
		for ( auto & it : m_list )
		{
			if ( it.get() )
			{
				if ( HasType<T>( * it.get() ) )	
					bResult = func( static_cast<T &>( * it.get() ) );
				if ( bResult )
					break;
			}
		}
		return bResult;
	}

private:
	vector<UPShape>     m_list;
	ShapeErrorHandler * m_pShapeErrorHandler { nullptr };

	void copy      ( ShapeList const & );
	void checkShape( Shape const & ) const;
};

UPShape ShallowCopy( Shape const & );
