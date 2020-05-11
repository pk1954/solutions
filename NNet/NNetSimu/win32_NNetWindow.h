// win32_NNetWindow.h : 
//
// NNetWindows

#pragma once

#include "MoreTypes.h"
#include "ShapeId.h"
#include "DrawContext.h"
#include "tHighlightType.h"
#include "SmoothMoveFp.h"
#include "win32_modelWindow.h"

using std::wstring;
using std::function;

class Scale;
class DrawModel;
class Observable;
class ActionTimer;
class AnimationThread;
class NNetModelInterface;
class NNetWorkThreadInterface;

class NNetWindow : public ModelWindow
{
public:
	static void InitClass
	( 
		NNetWorkThreadInterface * const,
		ActionTimer             * const
	);

	NNetWindow( );

	void Start
	( 
		HWND                 const, 
		DWORD                const,
		function<bool()>     const,
		NNetModelInterface * const,
		DrawModel          * const,
		Observable         * const,
		bool                 const
	);

	void Stop( );

	~NNetWindow( );

	ShapeId        const GetHighlightedShapeId( )          const { return m_shapeHighlighted; }
	ShapeId        const GetSuperHighlightedShapeId( )     const { return m_shapeSuperHighlighted; }
	tHighlightType const GetHighlightType( Shape const & ) const;

	void ResetHighlightedShape( ) { m_shapeHighlighted = NO_SHAPE; }
	void Zoom( bool const );
	void Zoom( MicroMeter const );
	void AnalysisFinished( );
	void CenterModel( bool const );

	MicroMeterPoint PixelPoint2MicroMeterPoint( PixelPoint const ) const;

	void PulseRateDlg   ( ShapeId const );
	void TriggerSoundDlg( ShapeId const );
	bool ChangePulseRate( bool const );
	void ShowDirectionArrows( bool const );

	DrawContext & GetDrawContext() { return m_context; }

private:

	NNetWindow             ( NNetWindow const & );  // noncopyable class 
	NNetWindow & operator= ( NNetWindow const & );  // noncopyable class 

	inline static NNetWorkThreadInterface * m_pNNetWorkThreadInterface { nullptr };
	inline static NNetModelInterface      * m_pModelInterface          { nullptr };

	HMENU m_hPopupMenu { nullptr };
	BOOL  m_bMoveAllowed { TRUE };    // TRUE: move with mouse is possible

	DrawContext       m_context              { };
	DrawModel       * m_pDrawModel           { nullptr };
	AnimationThread * m_pAnimationThread     { nullptr };
	Observable      * m_pCursorPosObservable { nullptr };

	PixelPoint m_ptLast            { PP_NULL };	// Last cursor position during selection 
	PixelPoint m_ptCommandPosition { PP_NULL };

	bool m_bFocusMode { false };
	bool m_bFixed     { false };  // Fixed windows cannot be moved or resized. Client area cannot be zoomed or moved

	MicroMeterPoint m_umPntCenterStart { MicroMeterPoint::NULL_VAL() }; // SmoothMove TODO: move these variables to SmootMoveFp
	MicroMeterPoint m_umPntCenterDelta { MicroMeterPoint::NULL_VAL() }; // SmoothMove 
	MicroMeter      m_umPixelSizeStart { MicroMeter::NULL_VAL() };      // SmoothMove 
	MicroMeter      m_umPixelSizeDelta { MicroMeter::NULL_VAL() };      // SmoothMove 
	SmoothMoveFp    m_smoothMove { };                                   // SmoothMove   

	PixelRect m_rectSelection { };

	ShapeId m_shapeHighlighted      { NO_SHAPE };
	ShapeId m_shapeSuperHighlighted { NO_SHAPE };

	virtual long AddContextMenuEntries( HMENU const, PixelPoint const );

	virtual void OnLeftButtonDblClick( WPARAM const, LPARAM const );
	virtual void OnMouseWheel        ( WPARAM const, LPARAM const );
	virtual void OnMouseMove         ( WPARAM const, LPARAM const );
	virtual BOOL OnCommand           ( WPARAM const, LPARAM const );
	virtual void OnLButtonUp         ( WPARAM const, LPARAM const );
	virtual bool OnRButtonUp         ( WPARAM const, LPARAM const );
	virtual void OnSetCursor         ( WPARAM const, LPARAM const );
	virtual void OnSize              ( WPARAM const, LPARAM const );
	virtual void OnLButtonDown       ( WPARAM const, LPARAM const );
	virtual bool OnRButtonDown       ( WPARAM const, LPARAM const );
	virtual void OnPaint( );

	void   smoothStep( );
	LPARAM crsPos2LPARAM( ) const;
	void   centerAndZoomRect( MicroMeterRect const, float const, bool const );
	LPARAM pixelPoint2LPARAM( PixelPoint const ) const;
	BOOL   inObservedClientRect( LPARAM const );
	void   doPaint( );
};
