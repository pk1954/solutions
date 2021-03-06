// win32_MainWindow.h : 
//
// NNetWindows

#pragma once

#include "Resource.h"
#include "scale.h"
#include "BaseKnot.h"
#include "MicroMeterPointVector.h"
#include "NNetModelReaderInterface.h"
#include "AlignAnimation.h"
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
	bool AlignSelection( ) { return m_upAlignAnimation->AlignSelection(); }
	void MakeConnector( );

	void       ShowArrows( bool const );
	bool const ArrowsVisible( ) const; 

	virtual bool OnCommand           ( WPARAM const, LPARAM const, PixelPoint const );
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

	unique_ptr<Animation<MicroMeter>> m_upArrowAnimation;
	MicroMeter                        m_arrowSizeTarget { STD_ARROW_SIZE };
	MicroMeter                        m_arrowSize       { m_arrowSizeTarget };

	unique_ptr<AlignAnimation>           m_upAlignAnimation;
	unique_ptr<Animation<PixelCoordsFp>> m_upCoordAnimation;

	MicroMeterRect      m_rectSelection        { };
	ShapeId             m_shapeHighlighted     { };
	ShapeId             m_shapeTarget          { };
	bool                m_bTargetFits          { false };
	Observable        * m_pCoordObservable     { nullptr };
	Observable        * m_pCursorPosObservable { nullptr };
	NNetModelCommands * m_pNNetCommands        { nullptr };
	
	void setTargetShape     ( );
	void setHighlightedShape( MicroMeterPoint const & );
	bool changePulseRate    ( ShapeId const, bool const );
	void centerAndZoomRect  ( UPShapeList::SelMode const, float const );

	void setNoTarget()
	{
		m_shapeTarget = NO_SHAPE;
		m_bTargetFits = false;
	}

	virtual void doPaint();
};
