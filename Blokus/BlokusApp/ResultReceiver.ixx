// ResultReceiver.ixx
//
// BlokusApp

export module ResultReceiver;

import BlokusCore;

export class ResultReceiver
{
    public:
        virtual void DeliverResult(PlayerId const) = 0;
        virtual bool TournamentRunning()           = 0;
};
