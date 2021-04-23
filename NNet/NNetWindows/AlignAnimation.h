//// AlignAnimation.h
////
//// NNetWindows
//
//#pragma once
//
//#include <vector>
//#include "ConnectionNeuron.h"
//#include "MicroMeterPointVector.h"
//#include "win32_animation.h"
//
//using std::vector;
//using std::unique_ptr;
//
//class NNetModelWriterInterface;
//class NNetModelCommands;
//class Sound;
//
//class AlignAnimation
//{
//public:
//	void Initialize
//	( 
//		HWND const,
//		NNetModelWriterInterface &,
//		NNetModelCommands &,
//		Sound *
//	);
//
//	static DWORD const ALIGN_DIRECTION  { 0x01 };
//	static DWORD const ALIGN_SHAPES     { 0x02 };
//	static DWORD const PACK_SHAPES      { 0x04 };
//	static DWORD const CREATE_CONNECTOR { 0x08 };
//
//	void                  StartAnimation( AnimationScript const & );
//	void                  AnimationStep();
//	bool            const NextScriptStep();
//	wchar_t const * const DoNextStep();
//
//private:
//
//	using ConnAnimation = Animation<MicroMeterPointVector>;
//
//	NNetModelWriterInterface * m_pNMWI          { nullptr };
//	NNetModelCommands        * m_pModelCommands { nullptr };
//	MicroMeterLine             m_line           { MicroMeterLine::NULL_VAL() };
//	DWORD                      m_animationPhase { AlignAnimation::ALIGN_DIRECTION };
//	Sound                   *  m_pSound         { nullptr };
//
//	AnimationScript    const * m_pScript { nullptr };
//	int                        m_iScriptStep { -1 };
//
//	unique_ptr<ConnAnimation> m_upConnAnimation;
//	ShapePtrList<ConnNeuron>  m_shapesAnimated;
//
//	bool         const prepareData();
//	void               doAnimation();
//	unsigned int const calcNrOfSteps(MicroMeterPointVector const &, MicroMeterPointVector const &) const;
//};
