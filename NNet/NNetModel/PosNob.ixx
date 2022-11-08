// PosNob.ixx
//
// NNetModel

module;

#include <type_traits>

export module NNetModel:PosNob;

import Types;
import :NobType;
import :Pipe;
import :Nob;

using std::is_base_of;
using std::remove_pointer_t;

export class PosNob : public Nob
{
public:
	PosNob(NobType const type)
	  : Nob(type)
	{ }

	virtual void ReplaceIncoming(Pipe* const, Pipe* const) = 0;
	virtual void ReplaceOutgoing(Pipe* const, Pipe* const) = 0;

	virtual void Apply2AllInPipes (PipeFunc const& f) const = 0;
	virtual void Apply2AllOutPipes(PipeFunc const& f) const = 0;

	virtual bool Apply2AllInPipesB (PipeCrit const& c) const = 0;
	virtual bool Apply2AllOutPipesB(PipeCrit const& c) const = 0;
};

export template <typename T>
concept PosNob_t = is_base_of<PosNob, remove_pointer_t<T>>::value;
