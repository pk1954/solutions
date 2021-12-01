// win32_NNetWindow.h : 
//
// NNetWindows

#pragma once

#include "MoreTypes.h"
#include "D2D_DrawContext.h"
#include "win32_modelWindow.h"

class ActionTimer;
class NNetController;
class NNetModelReaderInterface;

class NNetWindow : public ModelWindow
{
public:
	static void InitClass(ActionTimer * const);

	NNetWindow();
	~NNetWindow() override;

	void Start
	(
		HWND                     const, 
		DWORD                    const,
		bool                     const,
		fPixel                   const,
		NNetModelReaderInterface const &,
		NNetController                 &
	);

	void Stop();

	MicroMeterRect GetViewRect() const;

	DrawContext         & GetDrawContext()       { return m_context; }
	PixelCoordsFp const & GetCoordC     () const { return m_context.GetCoordC(); }
	PixelCoordsFp       & GetCoord      ()       { return m_context.GetCoord (); }
	MicroMeter            PixelSize     () const { return m_context.GetPixelSize(); }

	void DrawInteriorInRect  (PixelRect  const &, NobCrit const &  ) const;
	void DrawExteriorInRect  (PixelRect  const &, NobCrit const &  ) const;
	void DrawNeuronTextInRect(PixelRect  const &                   ) const;
	void DrawArrowsInRect    (PixelRect  const &, MicroMeter const ) const;

protected:

    void OnPaint      () override;
    bool OnSize       (WPARAM const, LPARAM const) override;
    bool OnCommand    (WPARAM const, LPARAM const, PixelPoint const) override;
    void OnLButtonDown(WPARAM const, LPARAM const)  override {}
   
    virtual void doPaint() = 0;

	void DrawSensors() const;
	void DrawHighlightedSensor() const;

	PixelPoint m_ptLast { PP_NULL };	// Last cursor position during selection 

	NNetModelReaderInterface const * m_pNMRI    { nullptr };
	D2D_driver                       m_graphics { };

private:
	NNetWindow             (NNetWindow const &);           // noncopyable class 
	NNetWindow & operator= (NNetWindow const &) = delete;  // noncopyable class 

	NNetController * m_pController { nullptr };
	D2D_DrawContext  m_context     { };
	fPixel           m_fPixRadiusLimit;
};
