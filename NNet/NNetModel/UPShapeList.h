// UPShapeList.h
//
// NNetModel

#pragma once

#include <array>
#include <vector>
#include <numeric>
#include "Shape.h"
#include "ShapeId.h"
#include "ShapePtrList.h"
#include "Connections.h"
#include "ShapeErrorHandler.h"

using std::move;
using std::array;
using std::vector;
using std::accumulate;
using std::unique_ptr;
using std::make_unique;

class ShapeIdList;

class UPShapeList
{
public:

	UPShapeList() { }
	UPShapeList( const UPShapeList & );
	~UPShapeList();

	UPShapeList & operator=  ( UPShapeList const & );
	bool          operator== ( UPShapeList const & ) const;

	bool    const IsEmpty       ()                 const { return m_list.size() == 0; }
	long    const Size          ()                 const { return Cast2Long( m_list.size() ); }
	ShapeId const IdNewSlot     ()	               const { return ShapeId( Cast2Long(m_list.size()) ); }
	bool    const IsShapeDefined(ShapeId const id) const { return GetAt( id ) == nullptr; }
	bool    const IsValidShapeId(ShapeId const id) const { return (0 <= id.GetValue()) && (id.GetValue() < Size()); }
	bool    const IsEmptySlot   (ShapeId const id) const { return GetAt( id ) == nullptr; }
	Shape * const Front         ()                 const { return   m_list[0].get(); }
	Shape * const GetAt         (int     const i ) const { return   m_list[i].get(); }
	Shape * const GetAt         (ShapeId const id) const { return   GetAt(id.GetValue()); }
	Shape       & GetRef        (int     const i )       { return * m_list[i]; }
	Shape       & GetRef        (ShapeId const id)       { return   GetRef(id.GetValue()); }
	void          Resize        (long    const nr)       { m_list.resize( nr );	}
	void          Increase      (long    const nr)       { m_list.resize( m_list.size() + nr ); }
				    
	void            Clear             ();
	void            SetErrorHandler   (ShapeErrorHandler * const);
	void            SelectAllShapes   (bool const);
	void            DeselectAllShapes () { SelectAllShapes(false); }
	ShapeId   const Push              (UPShape);
	UPShape         ExtractShape      (ShapeId const);	
	Shape   * const ReplaceShape      (ShapeId const, UPShape);	
	void            SetShape2Slot     (ShapeId const, UPShape);	 // only for special situations
	void            SetShape2Slot     (UPShape);                 // only for special situations
	ShapeType const DetermineShapeType()                                                        const;
	void            CheckShapeList    ()                                                        const;
	void            Dump              ()                                                        const;
	void            LinkShape         (Shape const &, function<Shape* (Shape const *)> const &) const;
	bool      const AnyShapesSelected ()                                                        const;
	void            CallErrorHandler  (ShapeId const )                                          const;
	ShapeId   const FindShapeAt       (MicroMeterPoint const, ShapeCrit const &)                const;
	bool      const Apply2AllB        (                       ShapeCrit const &)                const;
	void            Apply2All         (ShapeFuncC const & )                                     const;
	void            Apply2All         (ShapeFunc  const & );
	void            Apply2AllSelected (ShapeType const, ShapeFuncC const &)                     const;
	void            Apply2AllSelected (ShapeType const, ShapeFunc  const &);

	ShapeIdList Append       (UPShapeList &);
	UPShapeList ExtractShapes(ShapeIdList);

	enum class SelMode { allShapes,	selectedShapes };
	MicroMeterRect const CalcEnclosingRect(SelMode const = SelMode::allShapes) const;

	template <Shape_t T>
	unique_ptr<T> Pop()
	{
		unique_ptr<T> upT { unique_ptr<T>( static_cast<T*>(m_list.back().release()) ) };
		decCounter( upT->GetShapeType() );
		m_list.pop_back();
		return move( upT );
	}

	template <Shape_t T>    // const version
	void Apply2All( function<void(T const &)> const & func ) const
	{
		for ( auto const & it : m_list )
		{ 
			if ( it.get() && HasType<T>( * it.get() ) ) 
				func( static_cast<T const &>( * it.get() ) );
		};
	}                        

	template <Shape_t T>    // non const version
	void Apply2All( function<void(T &)> const & func )
	{
		for ( auto & it : m_list )
		{ 
			if ( it.get() && HasType<T>( * it.get() ) ) 
				func( static_cast<T &>( * it.get() ) );
		};
	}                        

	template <Shape_t T>    // const version
	void Apply2AllSelected( function<void(T const &)> const & func ) const
	{
		Apply2All<T>( {	[&](T const & s) { if ( s.IsSelected() ) func(s); } } );
	}

	template <Shape_t T>    // non const version
	void Apply2AllSelected( function<void(T &)> const & func ) 
	{
		Apply2All<T>( {	[&](T & s) { if ( s.IsSelected() ) func(s); } } );
	}

	template <Shape_t T>   // const version
	void Apply2AllInRect( MicroMeterRect const & r, function<void(T const &)> const & func ) const
	{
		Apply2All<T>( [&](T const & s) { if ( s.IsIncludedIn(r) ) func(s); } );
	}

	template <Shape_t T>   // non const version
	void Apply2AllInRect( MicroMeterRect const & r, function<void(T &)> const & func )
	{
		Apply2All<T>( [&](T & s) { if ( s.IsIncludedIn(r) ) func(s); } );
	}

	template <Shape_t T>
	bool Apply2AllB( function<bool(T &)> const & crit ) const
	{
		bool bResult { false };
		for ( auto & it : m_list )
		{
			if ( it.get() )
			{
				if ( HasType<T>( * it.get() ) )	
					bResult = crit( static_cast<T &>( * it.get() ) );
				if ( bResult )
					break;
			}
		}
		return bResult;
	}

	template <Shape_t T>
	ShapePtrList<T> GetAll(function<bool(T &)> const & crit)
	{
		ShapePtrList<T> shapePtrList;
		Apply2All<T>( [&](T &s)	{ if (crit(s)) shapePtrList.Add(&s); } );
		return move(shapePtrList);
	}

	template <Shape_t T>
	ShapePtrList<T> GetAllSelected()
	{
		return GetAll<T>( [&](T &s) { return s.IsSelected(); } );
	}

	template <Shape_t T>
	ShapePtrList<T> GetAllSelected(ShapeType const shapeType)
	{
		return GetAll<T>( [&](T &s) { return s.IsSelected() && s.HasType(shapeType); } );
	}

	unsigned int CountInSelection(ShapeType const shapeType) const
	{
		unsigned int uiNr { 0 };
		Apply2AllSelected( shapeType, [&](auto & s) { ++uiNr; } );
		return uiNr;
	}

	unsigned int const GetCounter(ShapeType const t) const 
	{ 
		return counter(t); 
	}

	unsigned int const GetCounter() const 
	{ 
		return accumulate( m_shapesOfType.begin(), m_shapesOfType.end(), 0 ); 
	}

private:

	unsigned int const & counter(ShapeType const t) const 
	{ 
		return m_shapesOfType[static_cast<unsigned int>(t.GetValue())]; 
	}

	unsigned int & counter(ShapeType const t)       
	{ 
		return const_cast<unsigned int &>(static_cast<const UPShapeList&>(*this).counter(t)); 
	}

	void incCounter(ShapeType const t) { ++counter(t); }
	void decCounter(ShapeType const t) { --counter(t); }

	void incCounter(Shape const * const ps) 
	{ 
		if (ps)
			incCounter(ps->GetShapeType()); 
	}
	void decCounter(Shape const * const ps) 
	{ 
		if (ps)
			decCounter(ps->GetShapeType()); 
	}

	void incCounter(UPShape const & ups) { incCounter(ups.get()); }
	void decCounter(UPShape const & ups) { decCounter(ups.get()); }

	void incCounter(ShapeId const & id)	{ incCounter(GetAt(id)); }
	void decCounter(ShapeId const & id) { decCounter(GetAt(id)); }

	void countShapes()
	{
		m_shapesOfType.fill( 0 );
		for ( auto & it : m_list )
		{ 
			ShapeType    const type  { it->GetShapeType() };
			unsigned int const index { static_cast<unsigned int>(type.GetValue()) };
			++m_shapesOfType[index];
		};
	}

	vector<UPShape> m_list;

	array<unsigned int, ShapeType::NR_OF_SHAPE_TYPES> m_shapesOfType {};

	ShapeErrorHandler * m_pShapeErrorHandler { nullptr };

	void copy      ( UPShapeList const & );
	void checkShape( Shape const & ) const;
};

UPShape ShallowCopy(Shape const &);

template <Shape_t T>
UPShape Copy(Shape const & shape) { return make_unique<T>( static_cast<T const &>(shape) ); }
