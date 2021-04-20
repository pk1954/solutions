// MakeConnectorCommand.h
//
// Commands

#pragma once

#include "Command.h"
#include "AlignAnimation.h"
#include "win32_animation.h"

class MakeConnectorCommand : public Command
{
public:
    MakeConnectorCommand(AlignAnimation & alignAnimation)
        : m_pAlignAnimation( & alignAnimation )
    {}

    virtual void Do( NNetModelWriterInterface& nmwi )
    {
        m_pAlignAnimation->AlignSelection(SCRIPT);
    }

    virtual void Undo( NNetModelWriterInterface& nmwi )
    {

    }

private:
    inline static AnimationScript const SCRIPT 
    {
        AlignAnimation::ALIGN_DIRECTION,  
        AlignAnimation::ALIGN_SHAPES,     
        AlignAnimation::PACK_SHAPES,      
        AlignAnimation::CREATE_CONNECTOR 
    };

    AlignAnimation * m_pAlignAnimation { nullptr };
};