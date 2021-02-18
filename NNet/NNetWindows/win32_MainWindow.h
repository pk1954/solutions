// win32_MainWindow.h : 
//
// NNetWindows

#pragma once

#include "Resource.h"
#include "scale.h"
#include "BaseKnot.h"
#include "MicroMeterPointVector.h"
#include "NNetModelReaderInterface.h"
#include "win32_animation.h"
#include "win32_NNetWindow.h"

class NNetModelCommands;

using std::unique_ptr;

class MainWindow : public NNetWindow
{
public:

	void Start
	(
		HWND                     const,
		DWORD                    const,
		bool                     const,
		fPixel                   const,
		NNetModelReaderInterface const &,
		MonitorWindow            const &, 
		NNetController                 &,
		NNetModelCommands              &,
		Observable                     &,
		Observable                     &
	);

	void Stop( );
	void Reset( );

	virtual long AddContextMenuEntries( HMENU const );

	ShapeId const GetTargetShapeId     ( ) const { return m_shapeTarget; }
	ShapeId const GetHighlightedShapeId( ) const { return m_shapeHighlighted; }

	virtual bool Zoom( MicroMeter const, PixelPoint const * const );

	void ZoomStep( bool const, PixelPoint const * const );

	template <typename T>
	void NNetMove( T const & delta )	
	{ 
		GetDrawContext().Move( delta ); 
		Notify( true );  
		if ( m_pCoordObservable )
			m_pCoordObservable->NotifyAll( false );
	}

	MicroMeterPoint const GetCursorPos( ) const;

	void CenterModel( );
	void CenterSelection( );
	void AlignSelection( );
	void MakeConnector( );

	void       ShowArrows( bool const );
	bool const ArrowsVisible( ) const; 

	virtual bool OnSize              ( WPARAM const, LPARAM const );
	virtual bool OnRButtonDown       ( WPARAM const, LPARAM const );
	virtual void OnMouseWheel        ( WPARAM const, LPARAM const );
	virtual void OnMouseMove         ( WPARAM const, LPARAM const );
	virtual void OnLeftButtonDblClick( WPARAM const, LPARAM const );
	virtual void OnLButtonUp         ( WPARAM const, LPARAM const );
	virtual bool OnRButtonUp         ( WPARAM const, LPARAM const );
	virtual void OnChar              ( WPARAM const, LPARAM const );
	virtual void OnPaint             ( );

private:
	 
	inline static MicroMeter const STD_ARROW_SIZE { 30.0_MicroMeter };
	
	Scale m_scale;

	using ArrowAnimation = Animation<MicroMeter,            MainWindow>;
	using CoordAnimation = Animation<PixelCoordsFp,         MainWindow>;
	using ShapeAnimation = Animation<MicroMeterPointVector, MainWindow>;

	unique_ptr<ArrowAnimation> m_upArrowAnimation;
	MicroMeter                 m_arrowSizeTarget { STD_ARROW_SIZE };
	MicroMeter                 m_arrowSizeRun    { m_arrowSizeTarget };

	unique_ptr<ShapeAnimation> m_upShapeAnimation;
	ShapePtrList<BaseKnot>     m_shapesAnimated;
	MicroMeterPointVector      m_umPntVectorRun;
	MicroMeterPointVector      m_umPntVectorTarget;

	unique_ptr<CoordAnimation> m_upCoordAnimation;

	MicroMeterRect      m_rectSelection        { };
	ShapeId             m_shapeTarget          { };
	ShapeId             m_shapeHighlighted     { };
	Observable        * m_pCoordObservable     { nullptr };
	Observable        * m_pCursorPosObservable { nullptr };
	NNetModelCommands * m_pNNetCommands        { nullptr };
	
	void setTargetShape     ( );
	void setHighlightedShape( MicroMeterPoint const & );
	bool changePulseRate    ( ShapeId const, bool const );
	void centerAndZoomRect  ( UPShapeList::SelMode const, float const );

	void alignedShapes();
	void setBaseKnots();

	virtual void doPaint();
};
