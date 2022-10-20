// BaseKnotFactory.ixx
//
// NNetModel

module;

#include <cassert>
#include <memory>

export module NNetModel:BaseKnotFactory;

import Types;
import :NobType;
import :BaseKnot;
import :Knot;
import :Neuron;
import :InputLine;
import :OutputLine;
import :Synapse;
import :Fork;
import :NNetModelWriterInterface;

using std::unique_ptr;
using std::make_unique;

export class BaseKnotFactory
{
public:
    static unique_ptr<BaseKnot> Make
	(
		MicroMeterPnt const& umPos, 
		NobType       const  nobType
	)
    {
		unique_ptr<BaseKnot> upBaseKnot { };
		switch (nobType.GetValue())
		{
			using enum NobType::Value;
			case knot:	     upBaseKnot = make_unique<Knot>      (umPos); break;
			case neuron:	 upBaseKnot = make_unique<Neuron>    (umPos); break;
			case inputLine:  upBaseKnot = make_unique<InputLine> (umPos); break;
			case outputLine: upBaseKnot = make_unique<OutputLine>(umPos); break;
			case synapse:    upBaseKnot = make_unique<Synapse>   (umPos); break;
			case fork:       upBaseKnot = make_unique<Fork>      (umPos); break;
			case undefined:	 break;
			default:         assert(false);
		}
		return move(upBaseKnot);
    }
};