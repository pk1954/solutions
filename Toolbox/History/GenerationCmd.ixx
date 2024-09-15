// GenerationCmd.ixx 
//
// GenerationCmd stores all necessary information to transform a model from generation n to generation n+1
// The size of GenerationCmd is 32 bit
//
// History

export module HistoryLib:GenerationCmd;

import std;
import Int24;
import Debug;
import SaveCast;
import :HistSlotNr;

using std::int8_t;
using std::wostream;

export class GenerationCmd
{
public:
	enum class Id : int8_t
	{
		UNDEFINED,
		CACHED,
		NEXT_GEN,
		UNDO,
		REDO,
		RESET,
		FIRST_APP_CMD
	};

	GenerationCmd()
		: m_Cmd(Id::UNDEFINED)
	{ }

	Id         GetCommand() const { return m_Cmd; }
	Int24      GetParam()   const { return m_Param; }
	HistSlotNr GetSlotNr()  const
	{
		Assert(m_Cmd == Id::CACHED);
		return HistSlotNr{ m_Param.GetValue() };
	}

	bool IsDefined()             const { return m_Cmd != Id::UNDEFINED; }
	bool IsUndefined()           const { return m_Cmd == Id::UNDEFINED; }
	bool IsCachedGeneration()    const { return m_Cmd == Id::CACHED; }
	bool IsNotCachedGeneration() const { return m_Cmd != Id::CACHED; }
	bool IsAppCommand()          const { return IsAppCmd(m_Cmd); }

	void InitializeCmd() { m_Cmd = Id::UNDEFINED; }

	static int const MIN_APP_CMD = static_cast<int>(Id::FIRST_APP_CMD);
	static int const MAX_APP_CMD = 0xff;

	static bool IsAppCmd(Id const cmd) { return cmd >= Id::FIRST_APP_CMD; }

	static GenerationCmd CachedCmd(HistSlotNr const slotNr)
	{
		return GenerationCmd(Id::CACHED, slotNr.GetValue());
	}

	static GenerationCmd NextGenCmd()
	{
		return GenerationCmd(Id::NEXT_GEN, 0);
	}

	static GenerationCmd ResetCmd(unsigned int const uiParam)
	{
		return GenerationCmd(Id::RESET, Int24(Cast2UnsignedInt(uiParam)));
	}

	static GenerationCmd ApplicationCmd(Id cmd, Int24 const param)
	{
		AssertLimits(static_cast<int>(cmd), MIN_APP_CMD, MAX_APP_CMD);
		return GenerationCmd(cmd, param);
	}

private:
	GenerationCmd(Id const cmd, Int24 const p) :
		m_Cmd(cmd),
		m_Param(p)
	{ }

	Id    m_Cmd;
	Int24 m_Param;
};

export wchar_t const* const GetGenerationCmdNameShort(GenerationCmd::Id const);
export wchar_t const* const GetGenerationCmdName     (GenerationCmd::Id const);

export wostream& operator << (wostream&, GenerationCmd const&);
