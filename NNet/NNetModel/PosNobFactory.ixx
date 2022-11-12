// PosNobFactory.ixx
//
// NNetModel

module;

#include <cassert>
#include <memory>

export module NNetModel:PosNobFactory;

import Types;
import :NobType;
import :PosNob;
import :Knot;
import :Neuron;
import :InputLine;
import :OutputLine;
import :Synapse;
import :Fork;
import :NNetModelWriterInterface;

using std::unique_ptr;
using std::make_unique;

export class PosNobFactory
{
public:
    static unique_ptr<PosNob> Make
	(
		MicroMeterPnt const& umPos, 
		NobType       const  nobType
	)
    {
		unique_ptr<PosNob> upPosNob { };
		switch (nobType.GetValue())
		{
			using enum NobType::Value;
			case knot:	     upPosNob = make_unique<Knot>      (umPos); break;
			case neuron:	 upPosNob = make_unique<Neuron>    (umPos); break;
			case inputLine:  upPosNob = make_unique<InputLine> (umPos); break;
			case outputLine: upPosNob = make_unique<OutputLine>(umPos); break;
			case undefined:	 break;
			default:         assert(false);
		}
		return move(upPosNob);
    }
};